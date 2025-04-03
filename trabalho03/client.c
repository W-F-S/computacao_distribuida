#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8081
#define BUFFER_SIZE 1024

int main() {
    int sock;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];

    // Criação do socket do cliente
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("erro ao criar socket");
        exit(EXIT_FAILURE);
    }

    // Configuração do endereço do servidor
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Conectar ao servidor
    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Erro ao conectar ao servidor");
        close(sock);
        exit(EXIT_FAILURE);
    }


    // Receber mensagem de conexão do servidor
    int n = recv(sock, buffer, BUFFER_SIZE - 1, 0);
    if (n > 0) {
        buffer[n] = '\0';
        printf("%s", buffer);
    }

    // Receber resposta de início ou erro
    n = recv(sock, buffer, BUFFER_SIZE - 1, 0);
    if (n > 0) {
        buffer[n] = '\0';
        printf("%s", buffer);

        // Se contiver mensagem de erro, sair
        if (strstr(buffer, "Erro:") != NULL) {
            close(sock);
            exit(EXIT_FAILURE);
        }
    }

    // Receber dados do arquivo
    while ((n = recv(sock, buffer, BUFFER_SIZE, 0)) > 0) {
        char *success_marker = strstr(buffer, "Arquivo enviado com sucesso.");
        if (success_marker != NULL) {
            size_t bytes_to_print = success_marker - buffer;
            if (bytes_to_print > 0) {
                fwrite(buffer, 1, bytes_to_print, stdout);
            }
            break;
        } else {
            fwrite(buffer, 1, n, stdout);
        }
    }


    close(sock);
    return 0;
}
