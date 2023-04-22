#include <stdio.h>
#include <stdlib.h>
#include <time.h>



int **geraMatriz(int lin, int col) {
    int **matriz = (int **) malloc(lin * sizeof(int *));
    for (int i = 0; i < lin; i++) {
        matriz[i] = (int *) malloc(col * sizeof(int));
        for (int j = 0; j < col; j++) {
            matriz[i][j] = 0;
        }
    }
    return matriz;
}


int **lerMatriz(FILE *arquivo, int *l,int *c) {
    int linha, coluna;
    fscanf(arquivo, "%d %d", &linha, &coluna);
    *l = linha;
    *c = coluna;
    int **matriz = (int **) malloc(linha * sizeof(int *));
    for (int i = 0; i < linha; i++) {
        matriz[i] = (int *) malloc(coluna * sizeof(int));
        for (int j = 0; j < coluna; j++) {
            char posicao[3];
            int valor;
            fscanf(arquivo, "%s %d", posicao, &valor);
            int linha_posicao = posicao[1] - '0';
            int coluna_posicao = posicao[2] - '0';
            matriz[linha_posicao][coluna_posicao] = valor;
        }
    }
    return matriz;
}

int **multiplicaMatriz(int **matriz1, int **matriz2, int lin1, int col1, int lin2, int col2) {
    if (col1 != lin2) {
        printf("Erro! O número de colunas da primeira matriz\n deve ser igual ao número de linhas da segunda.\n");
        return NULL;
    }
    int **resultado = geraMatriz(lin1,col2);
    
    for (int i = 0; i < lin1; i++) {
        for (int j = 0; j < col2; j++) {
            for (int k = 0; k < col1; k++) {
                resultado[i][j] += matriz1[i][k] * matriz2[k][j];
            }
        }
    }

    return resultado;
}

void salvar(int **matriz, int linhas, int colunas, float tempo, char nome[]) {
    FILE *arquivo;
    arquivo = fopen(nome, "w");
    fprintf(arquivo, "%d %d\n", linhas, colunas);
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            fprintf(arquivo, "c%d%d %d\n",i,j,matriz[i][j]);
        }
    }
    fprintf(arquivo, "%f ",tempo);
    fclose(arquivo);
}




int main() {
    int l1,c1;
    FILE *arquivo1 = fopen("matriz1.txt", "r");
    int **matriz1 = lerMatriz(arquivo1,&l1,&c1);
    fclose(arquivo1);

    int l2,c2;
    FILE *arquivo2 = fopen("matriz2.txt", "r");
    int **matriz2 = lerMatriz(arquivo2,&l2,&c2);
    fclose(arquivo2);
    
    clock_t inicio = clock();
    int **resultado = multiplicaMatriz(matriz1,matriz2,l1,c1,l2,c2);
    clock_t fim = clock();
    
    float tempo_execucao = (float)(fim - inicio)/ (CLOCKS_PER_SEC/1000000.0); //Tempo em segundos
    
    salvar(resultado, l1, c2, tempo_execucao,"matriz3.txt");

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