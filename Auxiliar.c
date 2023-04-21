#include <stdio.h>
#include <stdlib.h>
#include <time.h>


int **geraMatriz(int lin, int col) {
    int **matriz = (int **) malloc(lin * sizeof(int *));
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
    int l1, c1, l2, c2;
    scanf("%d %d %d %d", &l1, &c1, &l2, &c2);

    srand(time(NULL));

    int **matriz1 = geraMatriz(l1, c1);
    salvar(matriz1, l1, c1, "matriz1.txt");

    int **matriz2 = geraMatriz(l2, c2);
    salvar(matriz2, l2, c2, "matriz2.txt");

    for (int i = 0; i < l1; i++) {
        free(matriz1[i]);
    }
        free(matriz1);

    for (int i = 0; i < l2; i++) {
        free(matriz2[i]);
    }
        free(matriz2);

    return 0;
}