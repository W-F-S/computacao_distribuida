#include "matriz.h"
#include <stdio.h>
#include <rpc/rpc.h>

int main() {
    CLIENT *cliente;
    matrizes entrada;
    matriz *resultado;

    cliente = clnt_create("localhost", PROGRAM_MATRIZ, VERSION_MATRIZ, "udp");
    if (cliente == NULL) {
        clnt_pcreateerror("Erro ao criar cliente");
        return 1;
    }

    // Matrizes de exemplo 3x3
    int a[3][3] = {{1, 2, 3},
                   {4, 5, 6},
                   {7, 8, 9}};
    int b[3][3] = {{9, 8, 7},
                   {6, 5, 4},
                   {3, 2, 1}};

    for (int i = 0; i < TAM; i++) {
        for (int j = 0; j < TAM; j++) {
            entrada.m1.dados[i][j] = a[i][j];
            entrada.m2.dados[i][j] = b[i][j];
        }
    }

    resultado = multiplicar_matrizes_1(&entrada, cliente);

    printf("Resultado da multiplicação:\n");
    for (int i = 0; i < TAM; i++) {
        for (int j = 0; j < TAM; j++) {
            printf("%d ", resultado->dados[i][j]);
        }
        printf("\n");
    }

    clnt_destroy(cliente);
    return 0;
}
