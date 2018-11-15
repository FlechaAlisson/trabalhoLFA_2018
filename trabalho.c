#include "trabalho.h"




void LoadArquivo(char *fn, estado *Estados, int *tam, int *finais, int *tam_final)
{
	FILE *f = fopen(fn,"r");
	if(!f)
		{
			printf("Erro na leitura do arquivo\n");
			return;
		}

	fscanf(f,"Q=%d%*c",tam); 				                  //lendo a quantidade de estados e ignorando (não sei pq, mas é necessário ter um argumento)


	int i ;
	char aux;


	while(aux != '\n')
	{
		fscanf(f,"%c",&aux);			                      //lendo a linhas q contem os finais
		if(isdigit(aux)){				        	          //pegando somente os numeros
			finais[(*tam_final)] = aux - '0';                 //transformando em int
			(*tam_final)++;
		}
	}	

	char *batata[100];
	fscanf(f,"%[^\n]%*c",batata);                            //lendo o alfabeto e ignorando,


	i = 0;
	 while(!feof(f))                                         //começa as transições
	{
		char lado;

		int indice;
		fscanf(f,"(q%d",&Estados[i].num);

		fscanf(f,",%c)",&aux);
		Estados[i].leitura = aux ;                            //o char diretamente na struct
		fscanf(f,"=(q%d",&Estados[i].qual_ir);                //lendo o próximo estado

		fscanf(f,",%c",&aux);                                 //lendo o q escreve
		Estados[i].escrita = aux;

		fscanf(f,",%c%*c%*c",&lado);                          //lendo o lado

		if(lado == 'D')
			Estados[i].direcao = DIREITA;
		else
			Estados[i].direcao = ESQUERDA;

		i++;
	}

	*tam = i;

}

//printa o vetor de estados
void PrintaVetorEstado(estado *estados, int tam)
{
	printf("num | leitura | direcao | escrita | qual_ir\n");
	printf("----+---------+---------+---------+---------\n");
	for (int i = 0; i < tam; ++i)
	{
		if (estados[i].direcao)
		{
			printf("%d   |    %c    |    D    |    %c    |    %d",estados[i].num , estados[i].leitura , estados[i].escrita, estados[i].qual_ir);
		}
		else
		{
			printf("%d   |    %c    |    E    |    %c    |    %d",estados[i].num , estados[i].leitura , estados[i].escrita, estados[i].qual_ir);
		}
 
		printf("\n");
	}
}

void printEstado(estado estados)
{
	if (estados.direcao)
	{
		printf("%d   |    %c    |    D    |    %c    |    %d    >",estados.num , estados.leitura , estados.escrita, estados.qual_ir);
	}
	else
	{
		printf("%d   |    %c    |    E    |    %c    |    %d    >",estados.num , estados.leitura , estados.escrita, estados.qual_ir);
	}
}




//printa a string na forma pedida
void printPos (char s[], int pos, int num){
	int i;
	
	for (i = 0; i < strlen(s); i++){
		if (i != pos){
			printf ("%c", s[i]);
		}else{
			printf ("%c", s[i]);
			printf ("[q%d]", num);
		}
	}
	printf("\n");
}


//acresenta as caracteres '*' e '$' no começo e no final respectivamente
char* AdequaString(char s[])
{
	char *string =(char*) malloc(sizeof(char) * strlen(s) + 3);
	
	string[0] = '*';
	for (int i = 1; i <= strlen(s) ; ++i)
	{
		string[i] = s[i - 1];
	}
	string[strlen(s) +1] = '$';
	string[strlen(s) + 2] = '\0' ;
	
	return string;
}

// processa a string na maquina de turing recursivamente
void MaquinaDeTuring(int atual, int pos_string, char *s,estado *vetor_estado, int *vetor_finais, int tam_estados, int tam_final)
{

	for (int i = 0;i < tam_estados; ++i)
	{

		if(vetor_estado[i].num == atual) // testa se a transição correta
		{
			if (vetor_estado[i].leitura == s[pos_string]) //testa se a letra é a mesma na posição indicada
			{	
				
				if (vetor_estado[i].direcao == DIREITA) // testa se o ponteiro deve ir pra direita ou para esquerda
				{
					//printEstado(vetor_estado[i]);
					s[pos_string] = vetor_estado[i].escrita;        // escreve na fita o que deve ser escrito
					printPos(s,pos_string ,vetor_estado[i].qual_ir); // mostra na tela o resultado do processamento
					pos_string++;
					return MaquinaDeTuring(vetor_estado[i].qual_ir,pos_string,s,vetor_estado,vetor_finais,tam_estados,tam_final);
				}else
				{   //faz a mesma coisa que em cima mas com o diferencial que decrementa a posição do cabeçote.
					//printEstado(vetor_estado[i]);
					s[pos_string] = vetor_estado[i].escrita;
					printPos(s,pos_string-2,vetor_estado[i].qual_ir);  //possível gambi. Deu certo
					pos_string--;
					return MaquinaDeTuring(vetor_estado[i].qual_ir,pos_string,s,vetor_estado,vetor_finais,tam_estados,tam_final);
				}
			}
		}
	}

}


int main(int argc, char const *argv[])
{
	estado Estados[MAX];
	int tam_estados;
	int finais[MAX], tam_finais = 0;
	char entrada[MAX];
	char* string;

	system("cls");
	printf("Maquina de Turing\n");
	printf("insira a palavra a ser processada:\n>");
	scanf("%[^\n]",entrada);
	printf("\n");
	printf("PROCESSAMENTO\n");
	LoadArquivo("entrada.txt",Estados,&tam_estados,finais,&tam_finais);
	string = AdequaString(entrada);
	printPos(string,0,0);
	MaquinaDeTuring(0,1,string,Estados,finais,tam_estados,tam_finais);
	free(string);
	//PrintaVetorEstado(Estados,tam_estados);

	return 0;
}	