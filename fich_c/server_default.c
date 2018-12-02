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
#define PERM 0666

/*------------------------------*/
/*            Imports           */
/*------------------------------*/
#include "../fich_h/server_default.h"
#include "../fich_h/medit_default.h"
#include "../fich_h/client_default.h"
/*------------------------------*/

int conta_users, *users, user_to_kill;

void sig_handler2(int signo)
{
	int encontrou, i;
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

int verifica_user(char *nomeFicheiro, char *username, char *exe){
	FILE *f;
	char auxiliar[9];
	if((f=fopen(nomeFicheiro,"r"))==NULL){
		fprintf(stderr, "%s: erro ao abrir o ficheiro\n",exe);
		return -1;
	}
	while(fscanf(f,"%s",auxiliar)==1){	
		if(strcmp(auxiliar, username)==0){
			return 1;
		}		
	}
	fclose(f);
	return 0;
}

void limpa(){
	printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
}

void settings(server *server){
	int c;
	limpa();
	printf("\nNumero maximo de linhas: %d", server->MEDIT_MAXLINES);
	printf("\nNumero maximo de colunas: %d", server->MEDIT_MAXCOLUMNS);
	printf("\nNome do ficheiro da base de dados: %s", server->MEDIT_FICHEIRO);
	printf("\nNumero maximo de users a editar: %d", server->MEDIT_MAXUSERS);
	printf("\nNumero de pipes: %d", server->MEDIT_NUM_PIPES);
	printf("\nO nome da named pipe principal e: %s\n",server->MEDIT_NAME_PIPE_PRINCI);
	printf("\nClique numa tecla para sair...");
	c=getchar();
	c=getchar();
}

void kill_thread(){
	int i;
	for(i=0; i < conta_users; i++){
		kill(users[i], SIGUSR1);
	}
	free(users);
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
		printf("\nCliente com PID %d acabou de iniciar sessao\n", recebe.pid);
		//Fazer verificações para a alocação de memória
		user_to_kill=recebe.pid;
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
		strcpy(resposta.fifo_serv, "pipe1");
		sprintf(pipePID, "%d", recebe.pid);
		if( (fd_abrirE=open(pipePID, O_WRONLY))<0){
			fprintf(stderr, "Erro ao abir a pipe cliente\n");
			break;
		}
		resposta.muda=1;
		resposta.valID=verifica_user(recebe.ficheiro, recebe.nome, "client");
		//vai devolver -1 se não abrir o ficheiro
		//vai devolver 1 no caso de dar certo 
		//vai devolver 0 no caso de dar errado
		nw = write(fd_abrirE, &resposta, sizeof(servCli));
	}	
}

void * le_pipe1 (void * arg){
	int fd, nr, fdpipe[2];
	fd= *(int*) arg;
    	cliServ recebe;
	server server;
	pipe (fdpipe);	
	busca_ambiente(&server);

	signal(SIGUSR1, kill_thread);
	signal(SIGUSR2, sig_handler2);

	if( (fd=open("pipe1", O_RDWR))==-1){
		fprintf(stderr, "\nErro ao abir a pipe de leitura\n");
	}

	while((nr = read(fd, &recebe, sizeof(cliServ)))>0){
		printf("\nAlteracao do cliente %d:\n", recebe.pid);
		printf("\n%s\n", recebe.Frase);
		switch(fork()){
		case -1:
			fprintf(stderr, "\nErro no fork()\n");
			break;
		case 0:
			close(fdpipe[0]);
			dup2(fdpipe[1], STDOUT_FILENO);
			execlp("ls", "ls", NULL);
			fprintf(stderr, "\nErro no execlp()\n");
			break;
		default:
			close(fdpipe[1]);
			dup2(fdpipe[0], STDIN_FILENO);
			execlp("more", "more", NULL);
			fprintf(stderr, "\nErro no execlp()\n");
			break;
		}		
	}	
}
