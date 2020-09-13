#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// --------------------------------------------------------------------------------------------------------------------------

#define kN 5 // Matriz quadrada de NxN, dimensao N da matriz 
#define kNRO_PROCESSOS 4 // Numero P de processos 
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
            mat[(i*kN) + j] =(int) rand() % max+1;

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

void multLogica(int *matA, int *matB, int *matResposta) {
    int i,j,k;

    int minAux[kN];
    int cont = 0;

    for(i=0;i<kN;i++) {
        for(j=0;j<kN;j++) {
           for(k=0,cont=0;k<kN;k++,cont++) {
                minAux[cont] = min(matA[(j*kN)+k],matB[(k*kN)+i ]);
           }

           matResposta[(j*kN)+i] = maximo(minAux);
        }
    }
}

int main() {

    #if(kNRO_PROCESSOS > kN)
        #error Nro. processos deve ser menor ou igual a dimensao da matriz
    #endif

    //ALOCA MATRIZES
    int *mat1 =(int *) malloc (kN * kN * sizeof (int));
    int *mat2 = (int *) malloc (kN * kN * sizeof(int));
    int *mat3 = (int *) malloc (kN * kN * sizeof(int));

    srand(time(NULL));

    //GERA MATRIZES
    geraMatrizRandom(mat1, 100);
    geraMatrizRandom(mat2, 100);

    multLogica(mat1,mat2,mat3);

#ifdef kDEBUG
    //PRINT MATRIZES
    printf("\nPrintando Matriz 1:\n");
    printMatriz(mat1);

    printf("\nPrintando Matriz 2:\n");
    printMatriz(mat2);

    printf("\nPrintando Matriz Resultado:\n");
    printMatriz(mat3);
#endif

    //LIBERA ESPACO ALOCADO
    free(mat1);
    free(mat2);
    free(mat3);
}

// --------------------------------------------------------------------------------------------------------------------------