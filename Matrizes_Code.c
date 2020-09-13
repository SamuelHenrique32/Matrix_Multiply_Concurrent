#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h> // Criar processos
#include <sys/shm.h> // Mem compartilhada
#include <sys/wait.h>

// --------------------------------------------------------------------------------------------------------------------------

#define kN 10 // Matriz quadrada de NxN, dimensao N da matriz
#define kNRO_PROCESSOS 4 // Numero P de processos
#define kVALOR_MAX_CAMPO_MATRIZ 100
#define kPERMISSAO_MEM_COMPARTILHADA 0600
#define kCHAVE_MEM_COMPARTILHADA 3
#define kDEBUG

// --------------------------------------------------------------------------------------------------------------------------

int max(int a, int b) {
    if(a>=b)
        return a;

    return b;
}

int maximo(int minimos[kN]) {

    int maximo = max(minimos[0],minimos[1]);

    for(int i=2;i<kN;i++) {
        maximo = (max(maximo,minimos[i]));
    }

    return maximo;
}

int min(int a, int b) {

    if(a<=b)
        return a;

    return b;
}

void geraMatrizRandom(int *mat, int max) {

    int i,j;

    for(i=0;i<kN;i++) {
        for(j=0;j<kN;j++) {
            mat[(i*kN) + j] = (int)rand() % (max+1);
        }
    }
}

void printMatriz(int *mat) {

    for(int i = 0;i<kN;i++) {
        for(int j = 0;j<kN;j++) {
           printf("|%d|", mat[(i*kN) + j]);
        }
        printf("\n");
    }
}

void multLogica(int linhaInicial, int nroProcessos, int *matA, int *matB, int *bufferResposta) {
    int i,j,k;

    int minAux[kN];
    int cont = 0;

    for(i=linhaInicial ; i<kN ; i+=nroProcessos) {
        for(j=0;j<kN;j++) {
           for(k=0,cont=0;k<kN;k++,cont++) {
                minAux[cont] = min(matA[(j*kN)+k],matB[(k*kN)+i ]);
           }

           bufferResposta[(j*kN)+i] = maximo(minAux);
        }
    }
}

int main() {

    #if(kNRO_PROCESSOS > kN)
        #error Nro. processos deve ser menor ou igual a dimensao da matriz
    #endif

	int *mat3 = NULL;
    int shmid, chave = kCHAVE_MEM_COMPARTILHADA, i = 0, pid, linhaInicial = 0;

    //ALOCA MATRIZES
    int *mat1 =(int *)malloc(kN * kN * sizeof(int));
    int *mat2 = (int *)malloc(kN * kN * sizeof(int));

    srand(time(NULL));

    //GERA MATRIZES
    geraMatrizRandom(mat1, kVALOR_MAX_CAMPO_MATRIZ);
    geraMatrizRandom(mat2, kVALOR_MAX_CAMPO_MATRIZ);

    shmid = shmget(chave, (sizeof(int)*(int)pow(kN,2)), kPERMISSAO_MEM_COMPARTILHADA | IPC_CREAT);

    mat3 = shmat(shmid, 0, 0);

    for(i=1 ; i<kNRO_PROCESSOS ; i++) {
        pid = fork();

        // Filho
        if(pid == 0) {
            linhaInicial = i;
            break;
        }
    }

    multLogica(linhaInicial, kNRO_PROCESSOS, mat1, mat2, mat3);

    // Filhos
    if(linhaInicial != 0) {
        shmdt(mat3);
    }
    // Pai
    else {

        for(i=1 ; i<kNRO_PROCESSOS ; i++) {
            wait(NULL);
        }

        #ifdef kDEBUG
        //PRINT MATRIZES
        printf("\nPrintando Matriz 1:\n");
        printMatriz(mat1);

        printf("\nPrintando Matriz 2:\n");
        printMatriz(mat2);

        printf("\nPrintando Matriz Resultado:\n");
        printMatriz(mat3);
        printf("\n");
        #endif

        shmdt(mat3);

        shmctl(shmid, IPC_RMID, 0);
    }

    //LIBERA ESPACO ALOCADO
    free(mat1);
    free(mat2);
}

// --------------------------------------------------------------------------------------------------------------------------