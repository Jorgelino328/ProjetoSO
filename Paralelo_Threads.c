#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct {
    int id;             // Thread ID
    int l1,c1,c2;
    int num_threads;    // Total number of threads
    int rows_per_thread;// Number of rows to multiply per thread
    int** matrix1;      // First input matrix
    int** matrix2;      // Second input matrix
} ThreadInfo;



int **geraMatriz(int rows, int cols) {
    int **matrix = (int **) malloc(rows * sizeof(int *));
    for (int i = 0; i < rows; i++) {
        matrix[i] = (int *) malloc(cols * sizeof(int));
        for (int j = 0; j < cols; j++) {
            matrix[i][j] = 0;
        }
    }
    return matrix;
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

void *multiplicaMatriz(void *arg)
{
    
    ThreadInfo* info = (ThreadInfo*) arg;

    int start_row = info->id * info->rows_per_thread;
    int end_row = start_row + info->rows_per_thread;
    int c1 = info->c1;
    int c2 = info->c2;
    int l1 = info->l1;
    int id = info->id;
    int **result = geraMatriz(l1,c2);
    
    clock_t inicio = clock();
    for (int i = start_row; i < end_row; i++) {
        for (int j = 0; j < c2; j++) {
            int sum = 0;
            for (int k = 0; k < c1; k++) {
                sum += info->matrix1[i][k] * info->matrix2[k][j];
            }
            result[i][j] = sum;
        }
    }
    clock_t fim = clock();
    
    float tempo_execucao = (float)(fim - inicio)/ (CLOCKS_PER_SEC/1000000.0); //Tempo em microsegundos

    char buf[13];
    snprintf(buf, 13, "matriz_%d.txt", id); 
    salvar(result, l1, c2, tempo_execucao, buf);
    
    pthread_exit(NULL);
}

int main(int argc, char *argv[]){
    int P;
    scanf("%d",&P);
    
    int l1,c1;
    FILE *arquivo1 = fopen("matriz1.txt", "r");
    int **matriz1 = lerMatriz(arquivo1,&l1,&c1);
    fclose(arquivo1);

    int rows_per_thread = (l1 + P - 1) / P;

    int l2,c2;
    FILE *arquivo2 = fopen("matriz2.txt", "r");
    int **matriz2 = lerMatriz(arquivo2,&l2,&c2);
    fclose(arquivo2);

    int **resultado = geraMatriz(l1,c2);

    /*printf("c1: %d\n", c1);
    printf("c2: %d\n", c2);
    printf("l1: %d\n", l1);
    printf("l1: %d\n", l2);
    */
    pthread_t threads[P];
    ThreadInfo thread_info[P];

    for (int i = 0; i < (l1 * c2)/P; i++){
        thread_info[i].id = i;
        thread_info[i].num_threads = P;
        thread_info[i].rows_per_thread = rows_per_thread;
        thread_info[i].matrix1 = matriz1;
        thread_info[i].matrix2 = matriz2;
        thread_info[i].c1 = c1;
        thread_info[i].c2 = c2;
        thread_info[i].l1 = l1;

        printf("Processo principal criando thread #%d\n", i);
        pthread_create(&threads[i], NULL, multiplicaMatriz, (void *)&thread_info[i]);
    }

    for (int i = 0; i < P; i++) {
        pthread_join(threads[i], NULL);
    }

    for (int i = 0; i < l1; i++) {
        free(matriz1[i]);
        free(resultado[i]);
    }
    free(matriz1);
    for (int i = 0; i < l2; i++) {
        free(matriz2[i]);
    }
    free(matriz2);

    free(resultado);
    return 0;
}
