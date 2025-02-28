#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>

void error(char *); 

int main(int argc, char *argv[]) 
{ 
    int sock, length, n; 
    struct sockaddr_in server, from; 
    struct hostent *hp; 
    char entrada[1024];
    char saida[1024]; 

    if (argc != 3) { 
        printf("Uso correto: servidor porta\n"); 
        exit(1); 
    } 

    sock= socket(AF_INET, SOCK_DGRAM, 0); 
    if (sock < 0) 
        error("socket"); 
  
    server.sin_family = AF_INET; 
    hp = gethostbyname(argv[1]); 
    if (hp==0) error("host desconhecido"); 
    bcopy((char *)hp->h_addr,(char *)&server.sin_addr,hp->h_length); 
    server.sin_port = htons(atoi(argv[2])); 
    length=sizeof(struct sockaddr_in); 

    bzero(saida,1024);
    strcpy(saida, "'''");
    n=sendto(sock,saida,strlen(saida),0,(struct sockaddr *)&server,length);


    while(1){
        printf("Digite a menssagem: ");
        bzero(saida,1024);
        fgets(saida,1024,stdin);
        n=sendto(sock,saida,strlen(saida),0,(struct sockaddr *)&server,length);

        if (n < 0) error("Sendto");

        bzero(entrada,1024);


        while(1){
            n = recvfrom(sock,entrada,1024,MSG_WAITALL,(struct sockaddr *)&from, &length);
            if (n < 0) error("recvfrom");

            //write(1,"ACK:  ",12);
            //write(1,entrada,n);
            if(strcmp(entrada, "") !=0){
                printf("%s\n", entrada);
                break;
            }
        }

    }

} 

void error(char *msg) 
{ 
    perror(msg); 
    exit(0); 
} 

