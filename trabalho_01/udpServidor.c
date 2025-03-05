#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define negrito "\033[1m"
#define amarelo "\x1B[33m"
#define branco  "\x1B[37m"

#define MAX_CLIENTES 10  // Número máximo de clientes

struct Cliente {
    struct sockaddr_in endereco;
    socklen_t tamanho;
};

struct Cliente clientes[MAX_CLIENTES];
int num_clientes = 0;


//poderia verificar o hostname de que o endereco vem
void adicionar_cliente(struct sockaddr_in *novo_cliente, socklen_t tamanho) {
    for (int i = 0; i < num_clientes; i++) {
        if (clientes[i].endereco.sin_port == novo_cliente->sin_port) {
            return;  // Cliente já registrado
        }
    }

    if (num_clientes < MAX_CLIENTES) {
        clientes[num_clientes].endereco = *novo_cliente;
        clientes[num_clientes].tamanho = tamanho;
        num_clientes++;
    }
}

/**
 * salvar no server a porta de cada cliente conectado, retransmitir a mensagem para cada cliente conectado
 */



int main(int argc, char *argv[])
{ 

    int sock, length, fromlen, n; 
    struct sockaddr_in server;//endereco do servidor 
    struct sockaddr_in from;//endereco do destino 
    char entrada[256]; //mensagem recebida do cliente
    char saida[256] = "MSG recebida no servidor"; 

    if (argc < 2) { 
        fprintf(stderr, "\nERRO: porta não informada! \nDigite: ./udpserver <porta>\n\n"); 
        exit(0); 
   		 } 


//cria o socket

    sock=socket(PF_INET, SOCK_DGRAM, 0); 
    if (sock < 0){ 
        printf("Erro em socket()"); 
        exit(0);}

//define as estruturas de end para socket   
    length = sizeof(server); // tamanho do end
    bzero(&server,length); 
//elimina espacos nulos da struct de end
    server.sin_family=AF_INET;// define a familia de protocolos 
    server.sin_addr.s_addr=INADDR_ANY; 
//associa ao end local
    server.sin_port=htons(atoi(argv[1])); 
//porta forneciada pelo usuario

//associa end ao socket

    if (bind(sock,(struct sockaddr *)&server,length)<0) 
        { printf("Erro em bind ()");
          close (sock);
          exit(0);}
   
   fromlen = sizeof(struct sockaddr_in);

   printf("\n%sServidor iniciado, aguardando msgs dos clientes...\n",negrito); 

//faz leitura
    while (1) {
        bzero(entrada,256); 
        n = recvfrom(sock,entrada,256,0,(struct sockaddr *)&from,&fromlen); 

        if(strlen(entrada) == 3 && strcmp(entrada,"'''") == 0){
            printf("Novo cliente\n");
            adicionar_cliente(&from, fromlen);
        }else{
            if (n < 0)
            {printf(" Erro em recvfrom()");
            close(sock);
            exit(0);}

            printf("\nMensagem: %s", amarelo);
            printf("%s", entrada);
            printf("%s", branco);
            printf("\nRecebida de: %s   Porta: %d\n",inet_ntoa(from.sin_addr), ntohs(from.sin_port));

            char str []="sair";
            while (strncmp(str,entrada,4) == 0)
            {close(sock);
                exit(0);}

            //envia resposa
            printf("Enviando mensagem para clientes");
            for(int i = 0; i < num_clientes; i++){
                //sprintf(saida, "%d", clientes[i].endereco.sin_port);
                if(clientes[i].endereco.sin_port != from.sin_port){
                    n = sendto(sock,entrada,256,0,(struct sockaddr *)&clientes[i].endereco, clientes[i].tamanho);
                    if (n < 0){
                            printf("sendto");
                            close (sock);
                            exit(0);}

                }
            }

            printf("\n%sAguardando msgs dos clientes...\n", negrito);
        }
    } 
}


/**
 * passar na string o nome de um arquivo, abrir o arquivo no servidor e enviar o conteudo do arquvio para o cliente
 * tentar rodar em computadores
 */
