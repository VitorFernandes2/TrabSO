#include <sys/types.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/errno.h>
#include <pthread.h>
#include <signal.h>
#include <sys/wait.h>
#include <ncurses.h>
#define PERM 0666

/*------------------------------*/
/*            Imports           */
/*------------------------------*/
#include "../fich_h/server_default.h"
#include "../fich_h/medit_default.h"
#include "../fich_h/client_default.h"
/*------------------------------*/

int conta_users = 0, *users, *contaClientes, *ocupantesL;
char **users_nome, **matriz, **matrizP;
static server server1;
static pthread_mutex_t *lock;

//ocupantesL está iniciada a -1
//existem tantas lock como linhas (uma por cada)

int contaPipes(){
	inicializaContadorClientes();
	return server1.MEDIT_NUM_PIPES;
}


void inicializaContadorClientes(){

	contaClientes= malloc(server1.MEDIT_NUM_PIPES*sizeof(int));

	for(int i = 0; i < server1.MEDIT_NUM_PIPES; i++)
	{

		contaClientes[i] = 0;

	}

}

void pipe_ini(int *myFifo, char *nomePipe){
    
	*myFifo=mkfifo(nomePipe, PERM);

	if(*myFifo==-1){
		if(errno!=EEXIST){
			fprintf(stderr, "\nErro no mkfifo()\n");
			exit(-1);
 		}
	}
}

void distribui_user_P(char *nPipe)
{
	int men, posMen;
	
	posMen = 0;
	men = contaClientes[0];

	for(int i = 1; i < server1.MEDIT_NUM_PIPES; i++){

		if(men < contaClientes[i])
		{

			men = contaClientes[i];
			posMen = i;

		}

	}

	sprintf(nPipe,"%d", posMen);
}

int verifica_user(char *nomeFicheiro, int pid, char *username, char *exe, char *nPipe){

	FILE *f;
	char auxiliar[9];
	int i, aux, l, minimo;

	if (conta_users == server1.MEDIT_MAXUSERS) {	//Já não pode entrar mais utilizadores
		return 0;
	}

	if((f=fopen(nomeFicheiro,"r"))==NULL){

		fprintf(stderr, "%s: erro ao abrir o ficheiro\n",exe);
		return -1;

	}

	while(fscanf(f,"%s",auxiliar)==1){

		if (strcmp(auxiliar, username)==0) {
			
			if (conta_users > 0) {
				
				for(i = 0; i < conta_users -1; i++)
				{

					if(strcmp(auxiliar, matriz[i])==0) //Verifica se existe já na matriz de utilizadores
					{

						fclose(f);
						return 0;

					}

				}

				for(i=0; i<server1.MEDIT_MAXUSERS; i++)
				{

					if(users[i] == -1)
					{

						distribui_user_P(nPipe);
						strcpy(users_nome[i], username);
						users[conta_users] = pid;
						conta_users++;		

					}

				}
				
				fclose(f);
				return 1;

			}			
			else	//Se não houver nenhum utilizador
			{

				distribui_user_P(nPipe);
				strcpy(users_nome[0],username);
				users[0] =  pid;
				conta_users++;	
				fclose(f);
				return 1;

			}			
			
		}

	}
	fclose(f);
	return 0;

}

void desliga_user(int pid)
{
	int i;

	for(i = 0; i < server1.MEDIT_MAXUSERS; i++)
	{

		if(users[i] == pid)
		{

			conta_users--;
			users[i] = -1;
			strcpy(users_nome[i], " ");

		}

	}
}

void limpa(){
	printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
}

void settings(){
	int c;
	limpa();
	printf("\nNumero maximo de linhas: %d", server1.MEDIT_MAXLINES);
	printf("\nNumero maximo de colunas: %d", server1.MEDIT_MAXCOLUMNS);
	printf("\nNome do ficheiro da base de dados: %s", server1.MEDIT_FICHEIRO);
	printf("\nNumero maximo de users a editar: %d", server1.MEDIT_MAXUSERS);
	printf("\nNumero de pipes: %d", server1.MEDIT_NUM_PIPES);
	printf("\nO nome da named pipe principal e: %s\n",server1.MEDIT_NAME_PIPE_PRINCI);
	printf("\nClique numa tecla para sair...");
	c=getchar();
}

void kill_thread(){
	int i;

	for(i=0; i < conta_users; i++){
		if(users[i] != -1)
			kill(users[i], SIGUSR1);
	}
	
	pthread_exit(0);
}

void * le_pipe (void * arg){
	int fd, nr, pid, nw, fd_abrirE, i,conta_vistos;
	fd= *(int*) arg;
	char pipePID[10];
    	cliServ recebe;
	servCli resposta;

	signal(SIGUSR1, kill_thread);

	if( (fd=open(server1.MEDIT_NAME_PIPE_PRINCI, O_RDWR))==-1){
		fprintf(stderr, "\nErro ao abir a pipe de leitura\n");
	}
	
	while((nr = read(fd, &recebe, sizeof(cliServ)))>0){

		if(recebe.estado == 0)
		{

			resposta.estado=0;
			sprintf(pipePID, "%d", recebe.pid);
			if( (fd_abrirE=open(pipePID, O_WRONLY))<0){
				fprintf(stderr, "Erro ao abir a pipe cliente\n");
				break;
			}
			resposta.pid=getpid();
			resposta.muda=1;
			resposta.valID=verifica_user(server1.MEDIT_FICHEIRO, recebe.pid,recebe.nome, "client", resposta.fifo_serv);
			//vai devolver -1 se não abrir o ficheiro
			//vai devolver 1 no caso de dar certo 
			//vai devolver 0 no caso de dar errado
			nw = write(fd_abrirE, &resposta, sizeof(servCli));

		}
		else
			if(recebe.estado == 2)
			{

				desliga_user(recebe.pid);
				//Sair do servidor
					//Tirar o pid
					//Tirar o nome 

			}

	}	
}

void backspaceServer(int x, int y, char c1)
{

    int i;
    char c;

	printf("%d %d\n", y, x);
	if(x>=0)
		for(i = x; i < server1.MEDIT_MAXCOLUMNS - 1; i++)
		{
			
			matriz[y][i]=matriz[y][i + 1];
			
		}
	matriz[y][server1.MEDIT_MAXCOLUMNS - 1] = ' ';

}

void deleteServer(int x, int y, char c1)
{

	int i;
	char c;
	printf("%d %d\n", y, x);
	for(i = x; i < server1.MEDIT_MAXCOLUMNS - 1; i++)
	{
		if(i < server1.MEDIT_MAXCOLUMNS - 2)
			matriz[y][i+1] = matriz[y][i + 2];
		else
			matriz[y][i+1] = ' ';
		
	}
	matriz[y][i]=matriz[y][server1.MEDIT_MAXCOLUMNS - 1];

}

void valida_textoServer(int x, int y, char c1)
{

    int i;
    char c;

	printf("%d %d\n",y, x);

	for(i = server1.MEDIT_MAXCOLUMNS - 1; i > x; i--)
	{		
		matriz[y][i] = matriz[y][i - 1];
	}
	
	matriz[y][x] = c1;

}

void * le_pipe1 (void * arg){
	int fd, fd2, nr, nw, myPID;
	int i=0, j, ver, nPipe;
	char palavra[200], c, myPipe[10];

	fd= *(int*) arg;
	servCli envia;
    	cliServ recebe;

	signal(SIGUSR1, kill_thread);


	while((nr = read(fd, &recebe, sizeof(cliServ)))>0){
		
		//nPipe= atoi(recebe->pipe_pull);
		/*if(pthread_mutex_trylock(&lock[nPipe]) != EBUSY){
			pthread_mutex_lock(&lock[nPipe]);*/	
			j=0;
			ver=0;
			//Ver se o mutex está ocupado
			//Se não estiver dá-se lock
			//Senão retorna-se algo de modo ao cliente sair do modo edição
			//POSSIVEL TRUQUE
			//retornar o valor de 10 (enter no caracter) para ele entrar e sair logo

			//Verifica se a linha está ocupada
		
			if(ocupantesL[recebe.linha] != recebe.pid && ocupantesL[recebe.linha] != -1){
			
				myPID=recebe.pid;
				sprintf(myPipe, "%d", myPID);

				if( (fd2=open(myPipe, O_WRONLY))==-1){
					fprintf(stderr, "\nErro ao abrir a pipe de leitura\n");
				}

				envia.estado=2;
				envia.muda=1;
				envia.perm=1;
				nw = write(fd2,&envia,sizeof(servCli));

				close(fd2);
		
			}
			else
			{
				printf("ola\n");
				printf("\n1->%d\n", ocupantesL[recebe.linha]);
				ocupantesL[recebe.linha] = recebe.pid;
				printf("\n2->%d\n", ocupantesL[recebe.linha]);	
			}

		

			if(ocupantesL[recebe.linha] == recebe.pid)
			{		

				if(recebe.caracter=='\n')
				{			
				
					for(i = 0; i < server1.MEDIT_MAXCOLUMNS; i++)
					{
						while(matriz[recebe.linha][i] == ' ' || matriz[recebe.linha][i] == '\n'){
							i++;
						}

						if(matriz[recebe.linha][i] == ' ' || matriz[recebe.linha][i] == '\n')
						{
							palavra[j]='\0';

							verificaErros(palavra, &c);
							if(c != '*'){
								ver++;
							} 

							while(matriz[recebe.linha][i] == ' ' || matriz[recebe.linha][i] == '\n'){
								i++;
							}
							i--;
							j=0;
						}				
						else
						{
							palavra[j] = matriz[recebe.linha][i];
							j++;
						}
					
					}	

					if(ver == 0)
					{

						strcpy(matrizP[recebe.linha], matriz[recebe.linha]);
						ocupantesL[recebe.linha] = -1;

					}
								
				}
				else	//se for letra ou caracter especial vai para a posição da matriz
				{
				
					//Fazer validações da matriz

					//caso seja del
					if(recebe.caracter == 74)
						deleteServer(recebe.coluna, recebe.linha, recebe.caracter);

					//Caso seja caracter
					else
						if(recebe.caracter >= 32 && recebe.caracter <= 126)
							valida_textoServer(recebe.coluna, recebe.linha, recebe.caracter);

					//Caso seja backspace
					else
						if(recebe.caracter == 7)
							backspaceServer(recebe.coluna, recebe.linha, recebe.caracter);

					//caso seja esc
					else
						if(recebe.caracter == 27){
							matriz[recebe.linha] = matrizP[recebe.linha];
							ocupantesL[recebe.linha] = -1;
						}		
						

					fprintf(stderr, "%s<-\n", matriz[recebe.linha]);
					//Correr todos os clientes e mandar os caracteres

				}

				myPID=recebe.pid;
				sprintf(myPipe, "%d", myPID);

				if( (fd2=open(myPipe, O_WRONLY))==-1){
					fprintf(stderr, "\nErro ao abrir a pipe de leitura\n");
				}
			
			
				if(ver>0){
					envia.estado=1;
					envia.muda=1;
					envia.perm=0;
					nw = write(fd2,&envia,sizeof(servCli));
				}
				else{
					envia.estado=1;
					envia.muda=1;
					envia.perm=1;
					nw = write(fd2,&envia,sizeof(servCli));
				}

				close(fd2);
			}
			//pthread_mutex_unlock(&lock[nPipe]);
		//}
		
	}

}

void verificaErros(char *palavra, char *c)
{

	int i=0, j, toaspell_pipe[2], fromaspell_pipe[2];
	char sugestao[200], token[200];
	const char sign[2]="\n\0";
	
	if( (pipe(toaspell_pipe)) == -1){
		fprintf(stderr, "\nErro ao criar a pipe toaspell_pipe\n");
	}
	if( (pipe(fromaspell_pipe)) == -1){
		fprintf(stderr, "\nErro ao criar a pipe fromaspell_pipe\n");
	}
		
	switch(fork()){
		case -1:
			fprintf(stderr, "\nErro no fork()\n");
			break;
		case 0:
			dup2(toaspell_pipe[0], STDIN_FILENO);
			dup2(fromaspell_pipe[1], STDOUT_FILENO);
			close(toaspell_pipe[1]);
			close(fromaspell_pipe[0]);
			execlp("aspell", "aspell", "-a", (char*) NULL);
			break;
		default:		
			close(toaspell_pipe[0]);
			write(toaspell_pipe[1], palavra, strlen(palavra)+1); 
			close(toaspell_pipe[1]);
			
			wait(NULL);

			close(fromaspell_pipe[1]);				
			read(fromaspell_pipe[0], sugestao, 200); 
			close(fromaspell_pipe[0]);	

			strcpy(token,strtok(sugestao,sign));

			while( token[0] != '\0') {
				switch(token[0]){
					case '&':
					case '*':
					case '+':
					case '#':
						*c=token[0];
						token[0]='\0';
						break;
					default:
						strcpy(token,strtok(NULL,sign));
						break;
				}			
			}		
			break;
	}	
}

void muda_server(char *arg)
{	
	server1.MEDIT_FICHEIRO = arg;	
}

void ambi()
{
	busca_ambiente(&server1);
}

void mudaNPipes(int arg)
{
	server1.MEDIT_NUM_PIPES = arg;
}

void mudaMainPipe(char *arg)
{
	server1.MEDIT_NAME_PIPE_PRINCI = arg;
}

void mostraUsers()
{
	int i;
	char c;

	printf("Lista de Utilizadores: \n\n");
	
	for(i = 0; i < conta_users; i++)
	{
		
		printf("Utilizador #%d: %s\n", i + 1, users_nome[i]);
		
	}

	printf("\nClique numa tecla para sair...");
	c=getchar();
}

void inicio_matriz()
{

	int i, j, l;

	users = (int *) malloc(server1.MEDIT_MAXUSERS * sizeof(int));

	users_nome = (char **) malloc(server1.MEDIT_MAXUSERS * sizeof(char *));

	for(i = 0; i < server1.MEDIT_MAXUSERS; i++)
	{
		users_nome[i] = (char *) malloc(9 * sizeof(char));
		strcpy(users_nome[i], " ");

		users[i] = -1;
	}

	lock = (pthread_mutex_t *) malloc(server1.MEDIT_NUM_PIPES * sizeof(pthread_mutex_t));

	for(l=0; l < server1.MEDIT_NUM_PIPES; l++){
		pthread_mutex_init(&lock[l], NULL);
	}


	ocupantesL = (int *) malloc(server1.MEDIT_MAXLINES * sizeof(int));

	matriz = (char **) malloc(server1.MEDIT_MAXLINES * sizeof(char *));

	for(i = 0; i < server1.MEDIT_MAXLINES; i++)
	{
		
		matriz[i] = (char *) malloc(server1.MEDIT_MAXCOLUMNS * sizeof(char));

	}

	matrizP = (char **) malloc(server1.MEDIT_MAXLINES * sizeof(char *));
	
	for(i = 0; i < server1.MEDIT_MAXLINES; i++)
	{
		
		matrizP[i] = (char *) malloc(server1.MEDIT_MAXCOLUMNS * sizeof(char));

	}
	
	for(i = 0; i < server1.MEDIT_MAXLINES; i++)
	{
		
		for(j = 0; j < server1.MEDIT_MAXCOLUMNS; j++)
		{
			matriz[i][j] = ' ';
			matrizP[i][j] = ' ';
		}
		ocupantesL[i] = -1;
	}
		
}

void liberta_matriz()
{

	for(int i = 0; i < server1.MEDIT_MAXLINES; i++)
    	free(matriz[i]);
	free(matriz);

}

void liberta_users()
{
	free(users);
	for(int i = 0; i < server1.MEDIT_MAXUSERS; i++)
    	free(users_nome[i]);
	free(users_nome);
}

void statistics()
{
	int contaPalavras=0, contaLetras = 0, contaCar = 0, *num_letras, contaLinhas=0, ver;
	char *letras, c;

	letras = (char *) malloc(sizeof(char));
	num_letras = (int *) malloc(sizeof(int));

	for(int i = 0; i < server1.MEDIT_MAXLINES; i++)
	{

		for(int j; j < server1.MEDIT_MAXCOLUMNS; j++)
		{

			if((matriz[i][j] >= 'a' && matriz[i][j] <= 'z') || (matriz[i][j] >= 'A' && matriz[i][j] <= 'Z'))
			{
				contaLetras++;
			}

			if(contaCar == 0)
			{			

				contaCar++;
				letras = (char *) realloc(letras, contaCar * sizeof(char));
				num_letras = (int *) realloc(num_letras, contaCar * sizeof(int));
				num_letras[contaCar - 1] = 1;
				letras[contaCar - 1] = matriz[i][j];

			}
			else
			{
				ver=0;

				for(int k = 0; k < contaCar; k++)
				{
					
					if(matriz[i][j] == letras[k])
					{
						num_letras[k]++;
						ver++;
					}

				}

				if(ver==0)
				{
					
					contaCar++;
					letras = (char *) realloc(letras, contaCar * sizeof(char));
					num_letras = (int *) realloc(num_letras, contaCar * sizeof(int));
					num_letras[contaCar - 1] = 1;
					letras[contaCar - 1] = matriz[i][j];

				}

			}

			while(matriz[i][j] == ' ' || matriz[i][j] == '\0')
				j++;

			if(j<server1.MEDIT_MAXCOLUMNS)
				if(matriz[i][j + 1] == ' ' || matriz[i][j + 1] == '\0')
				{
					contaPalavras++;
				}

		}

		if(ocupantesL[i]!=-1)
			contaLinhas++;

	}

	printf("\nEstatisticas:\n\n");
	printf("Numero de palavras: %d\n", contaPalavras);
	printf("Linhas em edição: %d\n", contaLinhas);
	printf("Numero de caracteres: %d\n", contaLetras);
	printf("5 caracteres mais comuns:\n");

	int array[5]= {-1, -1, -1, -1, -1};
	char arrayC[5] = {' ',' ',' ',' ',' '};

	for(int k = 0; k < contaCar; k++)
	{
		
		for(int j=0; j < 5; j++)
		{

			if(array[j] < num_letras[k])
			{

				for(int i = 4; i > j; i--)
				{
					
					array[i] = array[i - 1];
					arrayC[i] = arrayC[i - 1];

				}

				array[j] = num_letras[k];
				arrayC[j] = letras[k];
				
				break;

			}

		}

	}

	for(int i = 0; i < 5; i++){
		printf("%d - %c\n", i + 1, arrayC[i]);
	}

	printf("\nClique numa tecla para sair...");
	c=getchar();

	free(num_letras);
	free(letras);
}
