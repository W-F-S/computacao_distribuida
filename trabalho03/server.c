#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8081
#define BUFFER_SIZE 1024
#define ARQUIVO_ENVIO "img.hdr"


void *conexao_cliente(void *client_socket_ptr) {
    int client_socket = *(int *)client_socket_ptr;
    const char *file_name = ARQUIVO_ENVIO;

    free(client_socket_ptr);

    // Enviar mensagem de conexão
    char *connection_msg = "conexão estabelecida com o servidor.\n";
    send(client_socket, connection_msg, strlen(connection_msg), 0);

    FILE *file = fopen(file_name, "rb");
    if (file == NULL) {
        char *error_msg = "Erro: arquivo não encontrado.\n";
        send(client_socket, error_msg, strlen(error_msg), 0);
        close(client_socket);
        pthread_exit(NULL);
    }

    char *start_msg = "Enviando arquivo:\n";
    send(client_socket, start_msg, strlen(start_msg), 0);

    // Enviar o conteúdo do arquivo
    char buffer[BUFFER_SIZE];
    size_t n;
    while ((n = fread(buffer, 1, sizeof(buffer), file)) > 0) {
        if (send(client_socket, buffer, n, 0) < 0) {
            perror("Erro ao enviar arquivo");
            break;
        }
    }
    fclose(file);

    char *success_msg = "\n\n\narquivo enviado com sucesso.\n\n\n";
    send(client_socket, success_msg, strlen(success_msg), 0);

    close(client_socket);
    pthread_exit(NULL);
}

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == 0) {
        perror("erro ao criar socket");
        exit(EXIT_FAILURE);
    }

    //configurando porta
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);



    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Erro em bind()\n"); //perror mostra mais informacoes

        exit(EXIT_FAILURE);
    }

    if (listen(server_socket, 10) < 0) {
        perror("erro ao ouvir no listen do server");
        exit(EXIT_FAILURE);
    }

    printf("Servidor ouvindo na porta %d...\n", PORT);

    while (1) {
        //accept espera indefinidamente por um cliente para ser conectado
        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_len);
        if (client_socket < 0) {
            perror("Erro ao aceitar conexão");
            continue;
        }

        int *client_sock_ptr = malloc(sizeof(int));
        if (client_sock_ptr == NULL) {
            perror("Erro ao alocar memória");
            close(client_socket);
            continue;
        }
        *client_sock_ptr = client_socket;

        pthread_t thread_id;
        //cria nova thread e atrela a função handle_client para ela
        if (pthread_create(&thread_id, NULL, conexao_cliente, client_sock_ptr) != 0) {
            perror("Erro ao criar thread");
            free(client_sock_ptr);
            close(client_socket);
            continue;
        }

        pthread_detach(thread_id);
    }

    close(server_socket);
    return 0;
}
