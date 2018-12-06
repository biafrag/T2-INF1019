#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>

//Tabela de página conterá todas as infos importantes para substituição de páginas
typedef struct tabelaPagina {
    int R; //Página referenciada
    int M; //Página modificada
    int ultimoAcesso; //Ultimo acesso à memória
    int indiceNoVetor;

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
int * criaVetorPaginas(int tamVetPags) 
{
	int i;
        
	//Alocando memória para vetor de páginas
	int *vet = (int *)malloc(sizeof(int) * tamVetPags);
	if(vet == NULL) 
	{
		printf("Erro no malloc\n");
		exit(1);
	}
	//Inicializa vetor de páginas com -1
	for(i = 0; i < tamVetPags; i++) 
	{
		vet[i] = -1;
	}

	return vet;
}
TabelaPagina * criaVetTabelaPaginas(int tamPag)
{
        //Fazer calculo para descobrir tamanho 
        int tamTabela = pow(2, 32 - (int)(ceil(log2(tamPag*1000))));
        int i;
        TabelaPagina *vetTabelaPaginas;
  	    printf("O tamanho da tabela de paginas eh : %d\n",tamTabela);
        
        vetTabelaPaginas = (TabelaPagina*)malloc(sizeof(TabelaPagina)*tamTabela);
		
        if(vetTabelaPaginas == NULL) 
        {
			printf("Erro na alocacao do vetor de tabela de paginas\n");
			exit(1);
        }
        for(i = 0; i < tamTabela; i++)
        {
                vetTabelaPaginas[i].R = 0;
                vetTabelaPaginas[i].M = 0;            
                vetTabelaPaginas[i].ultimoAcesso = 0; 
                vetTabelaPaginas[i].indiceNoVetor = -1;
        }
        return vetTabelaPaginas;
}
int buscaOutroEspacoNoVetor(int *vetPag,int tamVetPags)
{
    int i;
    for(i = 0; i < tamVetPags; i++) 
    {
        if(vetPag[i] == -1)
        {
                return i;
        }	
    }
    return -1;
}
int buscaPaginaNaMemoria(int tamVetPags, int *vetPag, int indicePag)
{
	int i;
    for(i = 0; i < tamVetPags; i++) 
    {
        if(vetPag[i] == indicePag)
		{
			return i;
		}
	}
	return -1;
}
int removeLRU(TabelaPagina* vetTabelaPaginas,int* vetPag,int tamVetPags)
{
    int i;
    //O que tiver menor valor de último acesso é a que não é acessada há mais tempo
    int indiceMenorTempo = 0;
    int menorTempo = 1000000000;
    
    //Percorre vetor de paginas achando a que tem o menor tempo de último acesso
    for(i = 0; i < tamVetPags; i++)
    {
        if(vetTabelaPaginas[vetPag[i]].ultimoAcesso < menorTempo)
        {
            indiceMenorTempo = i;
            menorTempo = vetTabelaPaginas[vetPag[i]].ultimoAcesso;
        }
    }    
    return indiceMenorTempo;
}

int removeNRU(TabelaPagina* vetTabelaPaginas,int* vetPag,int tamVetPags)
{
	int i, criterio;
	int priorR[] = {0,0,1,1}, priorM[] = {0,1,0,1}; 
	
	for (criterio = 0; criterio < 4; criterio++)
	{
		for (i=0; i<tamVetPags; i++)
		{
			TabelaPagina pag = vetTabelaPaginas[vetPag[i]];
			// verifica se R e M seguem os critérios do algoritmo
			if (pag.R == priorR[criterio] && pag.M == priorM[criterio]) //sempre ou só se tempos forem iguais?
			{
				return i;
			}
		}
	}	
    return -1;
}
int escolherRemoverPagina(char * tipoAlgo,TabelaPagina* vetTabelaPaginas,int* vetPag,int tamVetPags)
{
    int pos;
    if (strcmp(tipoAlgo,"LRU")==0)
    {
        pos = removeLRU(vetTabelaPaginas,vetPag,tamVetPags);
    }
    else
    {
        pos = removeNRU(vetTabelaPaginas,vetPag,tamVetPags);
    }
    return pos;
}
void zeraReferencias(int tamVetPags,TabelaPagina * vetTabelaPag, int* vetPag)
{
	int i;
	int indice;
	
	for(i = 0; i < tamVetPags; i++) {
		indice = vetPag[i];
		if(indice != -1) 
		{
			vetTabelaPag[indice].R = 0;
		}
	}
}

//Esperando entradas no terminal como:
// sim-virtual LRU arquivo.log 16 128
int main(int argc, char *argv[])
{
	// LRU ou NRU
	// Tamanhos de pagina : 8 a 32 K
	//Memória fisica: 1Mb a 16 Mb
    int proxPos = 0;
	FILE *entrada;
	unsigned int addr;
	char tipoAlgo[5], path[30];
	int tamPag, tamMemoFis,tamVetPags;
	int pageFault = 0; //Contador de page fault
	int pageWritten = 0; //Contador de páginas escritas 
	int debug = 0;
	int passo = 0;
    int flagEstaNaMemoria;
    unsigned int indicePag;
    char rw;
    int tempo = 0; //Tempo usado para conferir ultimo acesso
	int tempoZeraReferenciadas = 0; //Tempo para zerar as referenciadas

	int *vetPag; // vetor de páginas
	TabelaPagina *vetTabelaPaginas; //Vetor de tabela de página
	
	// Trata input
	strcpy(path,"logs/");

	strcpy(tipoAlgo, argv[1]); //pode ser LRU ou NRU

	strcat(path, argv[2]); //O path do .log
        
    printf("Path eh: %s\n",path);

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
	printf("\nExecutando o simulador...\n\n");
	//Cria vetor de páginas
    tamVetPags = tamMemoFis/tamPag; //Descobrindo tamanho do vetor
	vetPag = criaVetorPaginas(tamVetPags);

	// Cria vetor de tabela de paginas
	vetTabelaPaginas = criaVetTabelaPaginas(tamPag);
        
    entrada = fopen(path,"r");
    if (entrada == NULL)
    {
        printf("Erro ao abrir arquivo de entrada\n");
        exit(1);
    }
    //  Processo de leitura do arquivo
    while(fscanf(entrada,"%x %c",&addr,&rw) == 2) 
    {

		if (tempoZeraReferenciadas > tamVetPags) 
		{
			zeraReferencias(tamVetPags, vetTabelaPaginas, vetPag);
			tempoZeraReferenciadas = 0;
		} 

        //Conta para achar indice da página
        indicePag = addr >> (int)(ceil(log2(tamPag*1000)));
            
        //Verificando se indice já está memória
        
        flagEstaNaMemoria = buscaPaginaNaMemoria(tamVetPags, vetPag, indicePag); //vetTabelaPaginas[indicePag].indiceNoVetor;
        //printf("flagEstaNaMemoria:%d\n",flagEstaNaMemoria);
        
        if(debug || passo) 
        {
			int i;
			printf("Algoritmo: %s Time: %d\n", tipoAlgo, tempo);
			printf("Endereco: %x Modo: %c\n", addr, rw);
			printf("Indice: %d\n", indicePag);

			if(passo) 
		    {
				printf("Memoria:\n");
				printf("  ____________\n" );
				for(i = 0; i < tamVetPags; i++) 
		        {
					printf(" |  R:%d M:%d   |\n", vetTabelaPaginas[vetPag[i]].R, vetTabelaPaginas[vetPag[i]].M);
					printf(" |Ind:%-8d|\n", vetPag[i]);
					printf(" |____________|\n");
				}	
			}
		
			printf("Falta de pagina: %d\n", pageFault);
			printf("Paginas escritas: %d\n", pageWritten);
        }
            
        //Se a pagina não estiver já na memória
        if(flagEstaNaMemoria == -1)
        {
            pageFault++;
            int pos;
            
            //Procura espaco vazio no vetor
            pos = buscaOutroEspacoNoVetor(vetPag,tamVetPags);
                
            //Se tiver espaco vazio no vetor coloca página nele
            if(pos != -1 && proxPos+1 < tamVetPags)
            {
                vetPag[proxPos] = indicePag;
                vetTabelaPaginas[indicePag].indiceNoVetor = proxPos;
                proxPos++;
            }
            //Se não tiver mais espaço deveremos remover alguma página para dar espaço
            else
            {
                //Procurar pagina para ser removida dependendo do algoritmo desejado
                pos = escolherRemoverPagina(tipoAlgo,vetTabelaPaginas,vetPag,tamVetPags);
                if(debug || passo)
				{
                    printf("Posicao da pagina removida: %d\n", pos);
                }
                //Antes de trocar página guardar o status
                if(vetTabelaPaginas[vetPag[pos]].M == 1)
                {
                        //Se a página for modificada
                        pageWritten++;

                }
                    
                vetTabelaPaginas[vetPag[pos]].R = 0;
                vetTabelaPaginas[vetPag[pos]].M = 0;
                vetPag[pos] = indicePag;
                vetTabelaPaginas[indicePag].indiceNoVetor = pos;			
                    
        	}
        }
        else 
		{
			if(debug || passo) 
			{
				printf("Pagina ja esta na memoria.\n");
			}
		}        
        //Atualização da tabela dessa pagina
        vetTabelaPaginas[indicePag].ultimoAcesso = tempo;
        
        vetTabelaPaginas[indicePag].R = 1;
 
    
        //Se é write a página é modificada
        if(rw == 'W')
        {
            vetTabelaPaginas[indicePag].M = 1;
        }
            
        //Atualiza tempo
        tempo++;
                
        //Atualiza tempo para zerar
        tempoZeraReferenciadas++;
            
    }
    printf("Caminho do arquivo de entrada: %s\n", path); 
	
	if (tamMemoFis < 1000)
    	printf("Tamanho da memoria fisica: %d KB\n", tamMemoFis);
	else
		printf("Tamanho da memoria fisica: %.2f MB\n", tamMemoFis/1000.0);

	printf("Tamanho das paginas: %d KB\n", tamPag);
    printf("Algoritmo de substituicao: %s\n", tipoAlgo);
    printf("Número de falta de páginas: %d\n", pageFault);
    printf("Número de páginas escritas('sujas'): %d\n", pageWritten);
    
	printf("\n\nSimulação encerrada.\n");

    // Libera toda a memoria alocada

    free(vetTabelaPaginas);
    free(vetPag);
    fclose(entrada);

    return 0;
}
