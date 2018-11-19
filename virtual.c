#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
//Tabela de página conterá todas as infos importantes para substituição de páginas
typedef struct tabelaPagina {
	

} TabelaPagina;

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
int * criaVetorPaginas(int tamMemoFis,int paginaTam)
{
	int tamVetor,i;
	tamVetor = tamMemoFis/paginaTam;

	//Alocando memória para vetor de páginas
	int *vet = (int *)malloc(sizeof(int) * tamVetor);
	if(vet == NULL) 
	{
		printf("Erro no malloc\n");
		exit(1);
	}
	//Inicializa vetor de páginas com -1
	for(i = 0; i < tamVetor; i++) 
	{
		vet[i] = -1;
	}

	return vet;
}
TabelaPagina * criaVetTabelaPaginas(int tamPag)
{
	//Fazer calculo para descobrir tamanho
	return NULL;
}
int main(int argc, char *argv[])
{
	// LRU ou NRU
	// Tamanhos de pagina : 8 a 32 K
	//Memória fisica: 1Mb a 16 Mb

	FILE *arq;
	unsigned addr, page;
	char tipoAlgo[5], path[30];
	int tamPag, tamMemoFis;
	int pageFault = 0; //Contador de page fault
	int pageWritten = 0; //Contador de páginas escritas 
	int debug = 0;
	int passo = 0;

	int *vetPag; // vetor de páginas
	TabelaPagina *vetTabelaPaginas; //Vetor de tabela de página
	// Trata input
	strcpy(path,"arquivos/");

	strcpy(tipoAlgo, argv[1]); //pode ser LRU ou NRU

	strcat(path, argv[2]); //O path do .log

	tamPag =  atoi(argv[3]); //Tamanho da página pode ser 8 a 32Kb
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
	trataEntradas (tipoAlgo,tamPag,tamMemoFis);
	
	//Cria vetor de páginas
	vetPag = criaVetorPaginas(tamMemoFis,tamPag);

	// Cria vetor de tabela de paginas
	vetTabelaPaginas = criaVetTabelaPaginas(tamPag);

	return 0;
}
