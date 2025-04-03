#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <time.h>

#define PORT 33333

char *lerArq(char *vetor)
{
    if (strcmp(vetor, "data-hora.html") != 0)
    {
        char *vet = malloc(128);
        sprintf(vet, "HTTP/1.1 404 Not Found\r\nContent-Type: text/plain\r\n\r\nErro 404: Arquivo '%s' nao encontrado.", vetor);
        return vet;
    }

    time_t now = time(NULL);
    struct tm *local_time = localtime(&now);
    char *datetime = malloc(64);
    strftime(datetime, 64, "%d/%m/%Y %H:%M:%S", local_time);

    char *html = malloc(256);
    sprintf(html, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<html><head><title>Data e Hora</title></head><body><h1>Data e Hora Atual</h1><p>%s</p></body></html>", datetime);

    free(datetime);
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

        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
        {
            perror("accept()");
            exit(EXIT_FAILURE);
        }

        char buffer[30000] = {0};
        valread = read(new_socket, buffer, 30000);
        printf("%s\n", buffer);

        char *vet = malloc(64);
        for (int i = 0, j = 5; buffer[j] != ' '; i++, j++)
        {
            vet[i] = buffer[j];
        }

        char *html = lerArq(vet);
        write(new_socket, html, strlen(html));
        printf("Resposta Enviada");

        close(new_socket);
        free(html);
        free(vet);
    }

    return 0;}