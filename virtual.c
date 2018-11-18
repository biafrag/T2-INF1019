#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
void trataEntradas (char *tipoAlgo, int paginaTam,int tamMemoFis)
{
	//Testando algoritmo
	if(strcmp(tipoAlgo,"LRU")!=0 && strcmp(tipoAlgo,"NRU")!=0)
	{
		printf("Nao existe esse tipo de algoritmo, tente novamente\n");
		exit(1); 
	}

	//Testando intervalo de tamanho de pagina
	if(paginaTam < 8 || paginaTam > 32)
	{
		printf("Fora do intervalo de tamanho de pagina,tente novamente\n");
		exit(1);
	}

	//Checando intervalo da Memória fisica 
	if(tamMemoFis< 128 || tamMemoFis > 16*1024)
	{
		printf("Memoria fisica fora do range,tente novamente\n");
		exit(1);
	}
	printf("Entradas perfeitas\n");
	
}
int main(int argc, char *argv[])
{
	// LRU ou NRU
	// Tamanhos de pagina : 8 a 32 K
	//Memória fisica: 1Mb a 16 Mb

	FILE *arq;
	unsigned addr, page;
	char tipoAlgo[5], path[20];
	int paginaTam, tamMemoFis;
	int pageFault = 0; //Contador de page fault
	int pageWritten = 0; //Contador de páginas escritas 
	int debug = 0;
	int passo = 0;

	int *vetorPaginas;
	int vetorPaginasTam;

	// Trata input

	strcpy(tipoAlgo, argv[1]); //pode ser LRU ou NRU
	strcpy(path, argv[2]); //O path do .log

	paginaTam =  atoi(argv[3]); //Tamanho da página pode ser 8 a 32Kb
	tamMemoFis = atoi(argv[4]) ; //Tamanho da memória fisica pode ser de 1Mb a 16Mb

	if(argc > 5) 
	{
		int i;
		if(!strcmp(argv[5], "-d")) //Modo Debug
		{
			debug = 1;
			printf("Modo Debug\n");
		}
		else if(!strcmp(argv[5], "-p")) //Modo passo
		{
			passo = 1;
			printf("Modo Passo\n");
		}
		else //Não existente
		{
			printf("Opcao %s nao existe\n", argv[i]);
			exit(1);
		}

	}
	//Testa para ver se entradas estão corretas
	trataEntradas (tipoAlgo,paginaTam,tamMemoFis);

	return 0;
}
