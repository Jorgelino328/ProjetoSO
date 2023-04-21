#include <stdio.h>
#include <stdlib.h>
#include <time.h>


int **geraMatriz(int lin, int col) {
    int **matriz = (int **) malloc(lin * sizeof(int *));
    srand(time(NULL));
    for (int i = 0; i < lin; i++) {
        matriz[i] = (int *) malloc(col * sizeof(int));
        for (int j = 0; j < col; j++) {
            matriz[i][j] = rand() % 11; // Inteiro aleatório de 0 à 10
        }
    }
    return matriz;
}


void salvar(int **matriz, int linhas, int colunas, char nome[]) {
    FILE *arquivo;
    arquivo = fopen(nome, "w");
    fprintf(arquivo, "%d %d\n", linhas, colunas);
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            fprintf(arquivo, "c%d%d %d\n",i,j,matriz[i][j]);
        }
    }
    fclose(arquivo);
}

int main() {
    int r1, c1, r2, c2;
    scanf("%d %d %d %d", &r1, &c1, &r2, &c2);

    int **matriz1 = geraMatriz(r1, c1);
    salvar(matriz1, r1, c2, "matriz1.txt");

    int **matriz2 = geraMatriz(r2, c2);
    salvar(matriz2, r1, c2, "matriz2.txt");

    for (int i = 0; i < r1; i++) {
        free(matriz1[i]);
    }
        free(matriz1);

    for (int i = 0; i < r2; i++) {
        free(matriz2[i]);
    }
        free(matriz2);

    return 0;
}