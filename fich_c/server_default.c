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

int conta_users, *users, user_to_kill, *contaClientes;
char **users_nome, nome_to_kill[9], **matriz, **matrizP;
static server server1;

int contaPipes(){
	inicializaContadorClientes();
	return server1.MEDIT_NUM_PIPES;
}


void inicializaContadorClientes(){

	contaClientes= malloc(server1.MEDIT_NUM_PIPES*sizeof(int));

}

void sig_handler2(int signo)
{
	int encontrou, i, e;
	encontrou=i=0;
    	if (signo == SIGUSR2){        
        for(i=0; i<conta_users;i++){
			if(users[i]==user_to_kill){
				if(i!=conta_users-1){
					users[i]=users[i+1];
				}
				encontrou++;
			}
			else{
				if(encontrou>0)
					if(i!=conta_users-1)
						users[i]=users[i+1];
			}
		}

		encontrou = 0;
		
		for(i = 0; i < conta_users; i++)
		{
			
			if (strcmp(nome_to_kill, users_nome[i]) == 0) 
			{
				if(i != conta_users - 1)
					strcpy(users_nome[i], users_nome[i + 1]);
				encontrou++;
			}
			else
			{

				if(encontrou > 0)
					if(i != conta_users - 1)
						strcpy(users_nome[i], users_nome[i + 1]);
			}
			
		}
		
		conta_users--;

		users = (int*)realloc(users, conta_users*sizeof(int));
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

int verifica_user(char *nomeFicheiro, char *username, char *exe, char *nPipe){

	FILE *f;
	char auxiliar[9];
	int i, aux, l, minimo;

	if((f=fopen(nomeFicheiro,"r"))==NULL){

		fprintf(stderr, "%s: erro ao abrir o ficheiro\n",exe);
		return -1;

	}

	while(fscanf(f,"%s",auxiliar)==1){
		if(strcmp(auxiliar, username)==0){
			if(conta_users > 1)
				for(i = 0; i < conta_users; i++)
				{
					if(strcmp(users_nome[i],username)==0){
						conta_users--;
						users = (int *) realloc(users,conta_users * sizeof(int));
						fclose(f);
						return 0;
					}
						
				}			

			if(conta_users > server1.MEDIT_MAXUSERS)				
				return 0;	

			if(conta_users == 1)
			{				
				
				users_nome = (char **) malloc(server1.MEDIT_MAXUSERS * sizeof(char *));

				for(i = 0; i < server1.MEDIT_MAXUSERS; i++)
					users_nome[i] = (char *) malloc(9 * sizeof(char)); 

				for(i=0; i < 8; i++)
					users_nome[0][i]=username[i];				

				users_nome[0][8] = '\0';

				contaClientes[0]=1;
				sprintf(nPipe, "%d", 0);

			}
			else
				if(conta_users > 1){			

					for(i=0; i < 8; i++)
						users_nome[conta_users - 1][i]=username[i];				

					users_nome[conta_users - 1][8] = '\0';
			
					aux=0;
					minimo=contaClientes[0];
					for(l=1; l<=server1.MEDIT_NUM_PIPES; l++){
						if(contaClientes[l]<contaClientes[l-1] && contaClientes[l]<minimo){
							aux=l;
							minimo=contaClientes[l];
						}
					}
					sprintf(nPipe, "%d", aux);
				}

			return 1;

		}	

	}

	fclose(f);
	return 0;

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
	signal(SIGUSR2, sig_handler2);

	if( (fd=open(MEDIT_NAME_PIPE_PRINCI_V, O_RDWR))==-1){
		fprintf(stderr, "\nErro ao abir a pipe de leitura\n");
	}
	
	while((nr = read(fd, &recebe, sizeof(cliServ)))>0){
		//Fazer verificações para a alocação de memória
		user_to_kill=recebe.pid;
		
		for(i = 0; i < conta_users; i++)
			if(user_to_kill = users[i])
				strcpy(nome_to_kill, users_nome[i]);		
		
		if(conta_users==0){
			users = (int *) malloc(sizeof(int));
			conta_users++;
			users[0]=recebe.pid;
		}
		else{
			for(i=0;i<conta_users;i++){
				if(users[i]==recebe.pid){
					conta_vistos++;
					i=conta_users;
				}
			}
			if(conta_vistos){
				conta_vistos=0;
			}
			else{
				conta_users++;
				users = (int *) realloc(users, conta_users * sizeof(int));
				users[conta_users-1]=recebe.pid;
			}
		}

		resposta.estado=0;
		sprintf(pipePID, "%d", recebe.pid);
		if( (fd_abrirE=open(pipePID, O_WRONLY))<0){
			fprintf(stderr, "Erro ao abir a pipe cliente\n");
			break;
		}
		resposta.pid=getpid();
		resposta.muda=1;
		resposta.valID=verifica_user(server1.MEDIT_FICHEIRO, recebe.nome, "client", resposta.fifo_serv);
		//vai devolver -1 se não abrir o ficheiro
		//vai devolver 1 no caso de dar certo 
		//vai devolver 0 no caso de dar errado
		nw = write(fd_abrirE, &resposta, sizeof(servCli));
	}	
}

void escapeServer(int c, int y)
{

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
	for(i = server1.MEDIT_MAXCOLUMNS - 1; i > x; i--)
	{		
		matriz[y][i] = matriz[y][i - 1];
	}
	
	matriz[y][x] = c1;

}

void * le_pipe1 (void * arg){
	int fd, fd2, nr, nw, myPID;
	int i=0, j, ver;
	char palavra[200], c, myPipe[10];

	fd= *(int*) arg;
	servCli envia;
    	cliServ recebe;

	signal(SIGUSR1, kill_thread);
	signal(SIGUSR2, sig_handler2);


	while((nr = read(fd, &recebe, sizeof(cliServ)))>0){
		j=0;
		ver=0;
		printf("%d\n", recebe.caracter);
		if(recebe.caracter=='\n'){			

			for(i = 0; i < server1.MEDIT_MAXCOLUMNS; i++)
			{
				
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
				strcpy(matrizP[recebe.linha], matriz[recebe.linha]);			
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
				if(recebe.caracter == 27)		
					matriz[recebe.linha] = matrizP[recebe.linha];

			fprintf(stderr, "%s\n", matriz[recebe.linha]);
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
			strcpy(envia.fifo_serv,"0");
			envia.perm=0;
			nw = write(fd2,&envia,sizeof(servCli));
		}
		else{
			envia.estado=1;
			envia.muda=1;
			strcpy(envia.fifo_serv,"0");
			envia.perm=1;
			nw = write(fd2,&envia,sizeof(servCli));
		}
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

	int i, j;

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
}
