#include "matriz.h"
#include <stdio.h>

matriz *multiplicar_matrizes_1_svc(matrizes *entrada, struct svc_req *req) {
    static matriz resultado;
    int i, j, k;

    for (i = 0; i < TAM; i++) {
        for (j = 0; j < TAM; j++) {
            resultado.dados[i][j] = 0;
            for (k = 0; k < TAM; k++) {
                resultado.dados[i][j] += entrada->m1.dados[i][k] * entrada->m2.dados[k][j];
            }
        }
    }

    return &resultado;
}
