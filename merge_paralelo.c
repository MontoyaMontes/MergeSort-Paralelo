#include<stdlib.h> 
#include<stdio.h> 
#include <omp.h>

void merge(int arreglo[], int izq, int med, int der){ 
	//Inicializamos las variables auxiliares i,j,k.
	int i;
	int j;
	int k;

	//Inicializamos los nuevos índices.
	int aux1 = med - izq + 1;
	int aux2 = der - med;

	//Inicializamos los arreglos auxiliares. 
	int A1[aux1];
	int A2[aux2];

	for (i = 0; i < aux1; i++)
		A1[i] = arreglo[izq + i];

	for (j = 0; j < aux2; j++)
		A2[j] = arreglo[med + 1 + j];

	i = 0;
	j = 0;
	k = izq;

	while (i < aux1 && j < aux2){
		if (A1[i] <= A2[j]){
			arreglo[k] = A1[i];
			i++;
		} else {
			arreglo[k] = A2[j];
			j++;
		}
		k++;
	}

	while (i < aux1){
		arreglo[k] = A1[i];
		i++;
		k++;
	}

	while (j < aux2){
		arreglo[k] = A2[j];
		j++;
		k++;
	}
}

void parallel_mergeSort(int arreglo[], int izquierda, int derecha){
	//printf("Entrando índice %i con el índice %i a la sección paralela con hilo %i \n",izquierda,derecha,omp_get_thread_num());
	printf("Entrando elemento %i con el elemento %i a la sección paralela con hilo %i \n",arreglo[izquierda],arreglo[derecha],omp_get_thread_num());

	omp_set_num_threads(2);

	if (izquierda < derecha){
		int medio = izquierda + (derecha - izquierda)/2;
		#pragma omp parallel
		{
			//Aplicamos el metodo parallel_mergeSort sobre los dos sub-arreglos.
			if(omp_get_thread_num() == 0)
				parallel_mergeSort(arreglo, izquierda, medio);
			if(omp_get_thread_num() == 1)
				parallel_mergeSort(arreglo, medio+1, derecha);	
		}
		merge(arreglo, izquierda, medio, derecha);			
	}
}

int main(){
	//Arreglo inicial.
	int arreglo[] = {3, 4, 2, 1, 7, 5, 8, 9, 10, 6, 11, 12, 19};

	//Calculamos el tamaño del arreglo.
	int tamanio = sizeof(arreglo)/sizeof(int);

	//Imprimimos el arreglo inicial.
	int i;
	for (i=0; i < tamanio; i++)
		printf("%d ", arreglo[i]);
	printf("\n");

	//habilita paralelismo anidado
	omp_set_nested(1);

	parallel_mergeSort(arreglo, 0, tamanio - 1);

	//Imprimimos el arreglo ya ordenado.
	i = 0;
	for (i=0; i < tamanio; i++)
		printf("%d ", arreglo[i]);
	printf("\n");
	return 0;
}