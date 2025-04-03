#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <time.h>

#define PORT 33333

char *lerArq(const char *vetor)
{
    char *html;

    if (strcmp(vetor, "/data-hora.html") == 0)
    {
        printf("Solicitacao de data e hora\n");

        time_t now = time(NULL);
        struct tm *local_time = localtime(&now);
        char datetime[64];
        strftime(datetime, 64, "%d/%m/%Y %H:%M:%S", local_time);

        html = malloc(256);
        sprintf(html, "<html><head><title>Data e Hora</title></head><body><h1>Data e Hora Atual</h1><p>%s</p></body></html>", datetime);

        printf("Resposta: %s\n", html);
    }
    else if (strcmp(vetor, "/index.html") == 0)
    {
        printf("Solicitacao de index.html\n");

        html = strdup("<html><head><title>Exemplo de Botao</title><script>function obterDataHora() { var req = new XMLHttpRequest(); req.onreadystatechange = function() { if (this.readyState == 4 && this.status == 200) { document.getElementById('data-hora-atual').innerHTML = this.responseText; } }; req.open('GET', '/data-hora.html', true); req.send(); }</script></head><body><h1>Exemplo de Botao</h1><button onclick='obterDataHora()'>Obter Data e Hora Atual</button><div id='data-hora-atual'></div></body></html>");

        printf("Resposta: %s\n", html);
    }
    else
    {
        printf("Solicitacao invalida: %s\n", vetor);

        html = malloc(128);
        sprintf(html, "Erro 404: Arquivo '%s' nao encontrado.", vetor);

        printf("Resposta: %s\n", html);
    }

    return html;
}

int main(int argc, char const *argv[])
{
    int server_fd, new_socket;
    long valread;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket()");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    memset(address.sin_zero, '\0', sizeof address.sin_zero);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("bind()");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 10) < 0)
    {
        perror("listen()");
        exit(EXIT_FAILURE);
    }

    while (1)
    {
        printf("\nServidor Iniciado\n\n");

        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0)
        {
            perror("accept()");
            exit(EXIT_FAILURE);
        }

        char buffer[30000] = {0};
        valread = recv(new_socket, buffer, 30000, 0);
       printf("%s\n", buffer);

    char *vet = malloc(64);
    sscanf(buffer, "GET %s HTTP", vet);

    char *hello = lerArq(vet);

    char response_header[] = "HTTP/1.1 200 OK\nContent-Type: text/html\n\n";
    send(new_socket, response_header, sizeof(response_header) - 1, 0);
    send(new_socket, hello, strlen(hello), 0);

    printf("Resposta Enviada\n");
    close(new_socket);

    if (hello)
        free(hello);
    if (vet)
        free(vet);
}

return 0;}