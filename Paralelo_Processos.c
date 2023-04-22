#include <pthread.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>


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

void *multiplicaMatriz(int li_por_thread, int **matriz1, int **matriz2, int l1, int c1, int c2)

{   
    int id = getpid() - getppid();
    int l_comeco = id * li_por_thread;
    int l_final = l_comeco + li_por_thread;
    int **result = geraMatriz(l1,c2);
    clock_t inicio = clock();
    for (int i = l_comeco; i < l_final; i++) {
        for (int j = 0; j < c2; j++) {
            int sum = 0;
            for (int k = 0; k < c1; k++) {
                sum += matriz1[i][k] * matriz2[k][j];
            }
            result[i][j] = sum;
        }
    }
    clock_t fim = clock();
    /*float tempo_execucao = (float)(fim - inicio)/ (CLOCKS_PER_SEC); //Tempo em segundos

    char buf[60];
    snprintf(buf, 60, "Matrizes/Matrizes_dos_Processos/matriz%d.txt", id); */
    printf("Hello, im p#%d, child of #%d and i'm in a function\n",getpid(),getppid());
    //salvar(result, l1, c2, tempo_execucao, buf);
}

int main(int argc, char *argv[]){
    int P,status;
    scanf("%d",&P);
    
    int l1,c1;
    FILE *arquivo1 = fopen("Matrizes/Matrizes_Auxiliares/matriz1.txt", "r");
    int **matriz1 = lerMatriz(arquivo1,&l1,&c1);
    fclose(arquivo1);

    int li_por_thread = (l1 + P - 1) / P;

    int l2,c2;
    FILE *arquivo2 = fopen("Matrizes/Matrizes_Auxiliares/matriz2.txt", "r");
    int **matriz2 = lerMatriz(arquivo2,&l2,&c2);
    fclose(arquivo2);

    pid_t pid;
    int pai = getpid();
    for (int i = 0; i < P; i++){
        pid = fork();
        if(!pid){
            printf("Hello, im p#%d, child of #%d\n",getpid(),getppid());
            multiplicaMatriz(li_por_thread, matriz1, matriz2,l1,c1, c2);
            printf("Bye, im p#%d, child of #%d and i'm leaving now\n",getpid(),getppid());
            exit(0);
        }
    }
    while ((pid = wait(&status)) > 0) {
        printf("Filho %d terminou com status %d\n", pid, WEXITSTATUS(status));
    }

    for (int i = 0; i < l1; i++) {
        free(matriz1[i]);
    }
    free(matriz1);
    for (int i = 0; i < l2; i++) {
        free(matriz2[i]);
    }
    free(matriz2);

    printf("Bye, im p#%d, child of #%d and i'm leaving now\n",getpid(),getppid());
    
    return 0;
}
