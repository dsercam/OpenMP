#include <iostream>
#include <omp.h>
#include <cstdlib> 
#include <math.h>

#define SIZE 10000 // Tamaño de los arreglos de entrada
#define CHUNK 1000 // Definir el volumen de unidades en el que segmentaremos los arreglos
#define BUFFER 10  // Determinar cuantas posiciones imprimir


//Funcion para imprimir contenido de arreglos
//Recibe apuntador a un arreglo como parametro
void imprimirArreglo(float *d){
    //Imprimir N posiciones del arreglo al que se refiere el apuntador
    // El numero de posiciones es definido por el macro BUFFER.
    for (int i = 0; i < BUFFER; i++) printf("\t\t[%d] = %f \n",i,d[i]);
}

//Funcion para llenar los arreglos con numeros aleatorios (flotante)
//Recibe apuntador a un arreglo como parametro
void fillArrayWithRandomNumbers(float *d){
    //Iterar en toda la longitud del arreglo, definida por el macro SIZE, asignando un valor real aleatoriamente
    for (int i = 0; i < SIZE; i++) d[i] = static_cast <float> (rand() / (static_cast <float> (RAND_MAX/100.0)));
    //Informar al llenar el arreglo
    printf("\tArreglo llenado con exito.\n");
} 

//Funcion para liberar la memoria de los arreglos - especialmente necesaria si se implementa asignacion de memoria malloc, para arreglos grandes.
//Recibe apuntador a un arreglo como parametro
void cleanUp(float *d){
    delete d;
     //Informar al liberar memoria
    printf("\tMemoria liberada...\n");
}

int main(int argc, const char * argv[]) {
    //Validar en preprocesamiento si OMP esta disponible en el sistema
    #ifdef _OPENMP
        std::cout << "IMPORTANT! OMP disponible y funcionando" <<  std::endl;
        omp_set_num_threads(4);
    #endif
    //Informar si openMP no esta disponible
    #ifndef _OPENMP
        std::cout << "IMPORTANT! Configuracion de OMP inadecauda" << std::endl;
    #endif

    printf("Iniciando proceso. \n \tSe crearan arreglos con %d elementos...\n", SIZE);

    //Crear Arreglo A - tamaño definido por macro SIZE
    float *arrayA  = new float[SIZE];
    //Crear Arreglo B - tamaño definido por macro SIZE
    float *arrayB  = new float[SIZE];
    //Crear Arreglo C - tamaño definido por macro SIZE
    float *arrayC  = new float[SIZE];

    printf("\tLlenando arreglos con numeros aleatorios...\n", SIZE);
        fillArrayWithRandomNumbers(arrayA); //Usar funcion para llenar arrgelo A
        fillArrayWithRandomNumbers(arrayB); //Usar funcion para llenar arrgelo B

    //Visualizar los contenidos actuales de los arreglos de entrada
    printf("\nImprimiendo los primeros %d objetos del arreglo A...\n", BUFFER);   
    imprimirArreglo(arrayA); 
    printf("\nImprimiendo los primeros %d objetos del arreglo B...\n", BUFFER);   
    imprimirArreglo(arrayB);

    int j; //Iterador compartido
    printf("\nEJECUTANDO SUMA :: \t Arreglos con [%d] elementos se sumaran en paralelo en bathces de [%d] elementos \n", SIZE,CHUNK);
    //Usamos directiva de OMP para definir un ciclo for que buscamos ejecutar en paralelo
    //Definimos las estrucutras de datos que seran compartidas, los tres arreglos, y un iterador privado para cada uno de los threads
    #pragma omp parallel for shared(arrayA,arrayB,arrayC) private(j) schedule(static,CHUNK)
        for(j=0; j < SIZE;  j++) arrayC[j] = arrayA[j]+arrayB[j]; // Iteramos y salvamos en arreglo C la suma de A[j] y B[j]

    //Visualizar los contenidos del arreglo C, una vez realizada la suma.
    printf("\nImprimiendo los primeros %d objetos del arreglo C despues de la suma en paralelo...\n", BUFFER);   
    imprimirArreglo(arrayC); 

    // Clean up de la memoria.
    printf("\n\nLimpiando memoria alocada para los arreglos: \n");
    printf("Limpiza arreglo [A]"); cleanUp(arrayA);
    printf("Limpiza arreglo [B]"); cleanUp(arrayB);
    printf("Limpiza arreglo [C]"); cleanUp(arrayC);

    return 0;
    
}
