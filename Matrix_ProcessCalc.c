#include <stdio.h>

void getElementosMatriz(int matriz1[][10],int matriz2[][10], int row1, int column1,int row2, int column2) {

    printf("\nDigite os elementos da primeira matriz: \n");

    for (int i = 0; i < row1; ++i) {
        for (int j = 0; j < column1; ++j) {
            printf("Digite a(%d%d): ", i + 1, j + 1);
            scanf("%d", &matriz1[i][j]);
        }
    }

    printf("\nDigite os elementos da segunda matriz: \n");
    
    for (int i = 0; i < row2; ++i) {
        for (int j = 0; j < column2; ++j) {
            printf("Digite b(%d%d): ", i + 1, j + 1);
            scanf("%d", &matriz2[i][j]);
        }
    }
}

void multiplicaMatriz(  int first[][10],
                        int second[][10],
                        int result[][10],
                        int r1, int c1, int r2, int c2) {

    for (int i = 0; i < r1; ++i) {
        for (int j = 0; j < c2; ++j) {
            result[i][j] = 0;
        }
    }

    for (int i = 0; i < r1; ++i) {
        for (int j = 0; j < c2; ++j) {
            for (int k = 0; k < c1; ++k) {
                result[i][j] += first[i][k] * second[k][j];
            }
        }
    }
}

void mostrar(int result[][10], int row, int column) {

    printf("\nMatriz Resultado:\n");
    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < column; ++j) {
            printf("| %d ", result[i][j]);
            if (j == column - 1)
                printf("|\n");
        }
    }
}

int main() {
    int first[10][10], second[10][10], result[10][10], r1, c1, r2, c2;
    printf("Digite o número de linhas e colunas da primeira matriz:\n");
    printf("Linhas: ");
    scanf("%d", &r1);
    printf("Colunas: ");
    scanf("%d", &c1);
    printf("Digite o número de linhas e colunas da segunda matriz: \n");
    printf("Linhas: ");
    scanf("%d", &r2);
    printf("Colunas: ");
    scanf("%d", &c2);


   while (c1 != r2) {
        printf("Erro! O número de colunas da primeira matriz\n deve ser igual ao número de linhas da segunda.\n Por favor digite novamente:\n");
        printf("Digite o número de linhas e colunas da primeira matriz:\n");
        printf("Linhas: ");
        scanf("%d", &r1);
        printf("Colunas: ");
        scanf("%d", &c1);
        printf("Digite o número de linhas e colunas da segunda matriz: \n");
        printf("Linhas: ");
        scanf("%d", &r2);
        printf("Colunas: ");
        scanf("%d", &c2);
   }

    getElementosMatriz(first, second, r1, c1, r2 , c2 );

    multiplicaMatriz(first, second, result, r1, c1, r2, c2);

    mostrar(result, r1, c2);

    return 0;
}