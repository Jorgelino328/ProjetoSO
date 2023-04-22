#include <pthread.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>


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

void *multiplicaMatriz(int rows_per_thread, int **matriz1, int **matriz2, int l1, int c1, int c2, int p_id, int id)
{
    id = id - p_id;
    printf("Hello, im p#%d\n",id);
    int start_row = id * rows_per_thread;
    int end_row = start_row + rows_per_thread;
    int **result = geraMatriz(l1,c2);
    

   /* printf("rows_per_thread: %d\n",rows_per_thread);
    printf("start_row: %d\n",start_row);
    printf("end_row: %d\n",end_row);
    printf("l1: %d\n",l1);
    printf("c1: %d\n",c1);
    printf("c2: %d\n",c2);
    printf("p_pid: %d\n",id);
    printf("pid: %d\n",id);
    */


    clock_t inicio = clock();
    for (int i = start_row; i < end_row; i++) {
        for (int j = 0; j < c2; j++) {
            int sum = 0;
            for (int k = 0; k < c1; k++) {
                sum += matriz1[i][k] * matriz2[k][j];
            }
            result[i][j] = sum;
        }
    }
    clock_t fim = clock();
    
    float tempo_execucao = (float)(fim - inicio)/ (CLOCKS_PER_SEC/1000000.0); //Tempo em microsegundos

    char buf[13];
    snprintf(buf, 13, "matriz-%d.txt", id); 
    salvar(result, l1, c2, tempo_execucao, buf);
    exit(0);
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

    pid_t pid[P];
    int status;

        
    for (int i = 0; i < (l1 * c2)/P; i++){
        pid[0] = fork();   
        //printf("Hello, im p#%d\n",pid[i]);
        if(!pid[0]){
            multiplicaMatriz(rows_per_thread, matriz1, matriz2,l1,c1, c2,getppid(),getpid());
        }
    }

     
    while(wait(NULL) > 0);

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
