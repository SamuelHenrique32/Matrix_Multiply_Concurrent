#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include<time.h>


#define N 5
//Matriz quadrada de NxN


int max(int a, int b){
    if(a>=b)
        return a;
    return b;
}

int maximo(int minimos[N]){
    int maximo = max(minimos[0],minimos[1]);
    for(int i=2;i<N;i++){
        maximo = (max(maximo,minimos[i]));
    }
    return maximo;
}

int min(int a, int b){
    if(a<=b)
        return a;
    return b;
}

void geraMatrizRandom(int *mat, int max){
    int i,j;
    for(i=0;i<N;i++){
        for(j=0;j<N;j++){
            mat[(i*N) + j] =(int) rand() % max+1;

        }
    }
}

void printMatriz(int *mat){
    printf("Printando Matriz \n");
    for(int i = 0;i<N;i++){
        for(int j = 0;j<N;j++){
           printf("|%d|", mat[(i*N) + j]);
        }
        printf("\n");
    }
}

void multLogica(int *matA, int *matB, int *matResposta){
    int i,j,k;

    int minAux[N];
    int cont = 0;

    for(i=0;i<N;i++){
        for(j=0;j<N;j++){

           for(k=0,cont=0;k<N;k++,cont++){
            minAux[cont] = min(matA[(j*N)+k],matB[(k*N)+i ]);
           }
           matResposta[(j*N)+i] = maximo(minAux);
        }

    }

}

int main(){
//ALOCA MATRIZES
int *mat1 =(int *) malloc (N * N * sizeof (int));
int *mat2 = (int *) malloc (N * N * sizeof(int));
int *mat3 = (int *) malloc (N * N * sizeof(int));
srand(time(NULL));
//GERA MATRIZES
geraMatrizRandom(mat1, 100);
geraMatrizRandom(mat2, 100);

multLogica(mat1,mat2,mat3);
//PRINT MATRIZES
printMatriz(mat1);
printMatriz(mat2);
printMatriz(mat3);

//LIBERA ESPAÇO ALOCADO
free(mat1);
free(mat2);
free(mat3);




}
