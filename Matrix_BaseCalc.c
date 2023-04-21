#include <stdio.h>


int **multiplicaMatriz(int **matriz1, int **matriz2, int lin1, int col1, int lin2, int col2) {
    if (col1 != lin2) {
        printf("Erro! O número de colunas da primeira matriz\n deve ser igual ao número de linhas da segunda.\n");
        return NULL;
    }
    int **resultado = geraMatriz(lin1, col2);
    
    for (int i = 0; i < lin1; i++) {
        for (int j = 0; j < col2; j++) {
            for (int k = 0; k < col1; k++) {
                resultado[i][j] += matriz1[i][k] * matriz2[k][j];
            }
        }
    }

    return resultado;
}

int main() {
    clock_t inicio = clock();
    int **resultado = multiplicaMatriz(matriz1,matriz2,l1,c1,l2,c2);
    clock_t fim = clock();

    double tempo_execucao = fim - inicio;
    
    return 0;
}