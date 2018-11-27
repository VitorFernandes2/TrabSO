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
#define PERM 0777

#include "fich_h/server_default.h"
#include "fich_h/medit_default.h"

void limpa(){
	printf("\n\n\n\n\n\n\n\n\n\n\n");
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

void * le_pipe (void * arg){
	int fd, nr, pid, i, verifica_fifo;
	char istring[3], pipe[10];
	fd= (int) arg;

	if( (fd=open(MEDIT_NAME_PIPE_PRINCI_V, O_RDWR))==-1){
		fprintf(stderr, "\nErro ao abir a pipe de leitura\n");
		exit(-1);
	}
	
	nr = read(fd, &pid, sizeof(int));
	printf("\nCliente %d acabou de iniciar sessao\n", pid);
	
	strcpy(pipe, "pipe");
	for(i=0; i < MEDIT_NUM_PIPES_V; i++){
		sprintf(istring, "%d", i+1);
		strcat(pipe, istring);
		if( ( verifica_fifo= mkfifo (pipe, PERM) ) == -1 ){
			fprintf(stderr, "\nErro ao criar a fifo\n");
		}
		strcpy(pipe, "pipe");
	}
	
	
	pthread_exit(NULL);
	
}

int main(int argc, char *argv[]){
	server server;
	char cline[20], lixo, *user,hostname[20];
	user=getenv("USER");
	gethostname(hostname,20);
	int tamArgc, fd_server_pipe, verifica_fifo;
	pthread_t t_server;

	if(argc!=1){
		for(tamArgc=1; tamArgc<argc; tamArgc++){
			fprintf(stderr, "\n%s: comando invalido", argv[tamArgc]);
		}
		fprintf(stderr, "\nSintaxe: executavel\n");
		exit(-1);
	}

	verifica_fifo=mkfifo(MEDIT_NAME_PIPE_PRINCI_V, PERM);


	if(verifica_fifo==-1){
		if(errno==EEXIST){
			printf("\nA fifo %s ja existe\n", MEDIT_NAME_PIPE_PRINCI_V);
		}
		else {
			//Abort("\nErro no mkfifo()\n");
		}
	}
	else {
		printf("\nFifo %s criado\n", MEDIT_NAME_PIPE_PRINCI_V);
	}

	if((pthread_create(&t_server, NULL, le_pipe, (void *)&fd_server_pipe))==-1){
		fprintf(stderr, "\nErro: criacao da thread principal do server\n");

	}
		
	do{
		limpa();
		printf("\nServidor:");
		printf("\nsettings");
		printf("\nload <filename>");
		printf("\nsave <filename>");
		printf("\nfree <linenumber>");
		printf("\nstatistics");
		printf("\nusers");
		printf("\ntext");
		printf("\nshutdown");
		printf("\n\n%s@%s:~$ ",user,hostname);
		fflush(stdin);
		scanf("%s", cline);

		if(strcmp(cline, "settings")==0){		
			busca_ambiente(&server);
			settings(&server);
			fflush(stdout);
		}
		else
			if(strcmp(cline, "shutdown")!=0){			
				printf("%s: comando nao disponivel\n", cline);	
			}		
	}while(strcmp(cline, "shutdown")!=0);
	
	
	pthread_join(t_server, NULL);
	remove(MEDIT_NAME_PIPE_PRINCI_V);
	exit(0);
}
