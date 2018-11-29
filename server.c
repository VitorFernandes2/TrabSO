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

#include "fich_h/server_default.h"
#include "fich_h/medit_default.h"


int main(int argc, char *argv[]){
	server server;
	char cline[20], lixo, *user,hostname[20];
	gethostname(hostname,20);
	int tamArgc, fd_server_pipe, fifoPrincipal, fifoPull;
	pthread_t t_server;
	void *estado;

	user=getenv("USER");

	
	if(argc!=1){
		for(tamArgc=1; tamArgc<argc; tamArgc++){
			fprintf(stderr, "\n%s: comando invalido", argv[tamArgc]);
		}
		fprintf(stderr, "\nSintaxe: executavel\n");
		exit(-1);
	}

	pipe_ini(&fifoPrincipal, MEDIT_NAME_PIPE_PRINCI_V);
	pipe_ini(&fifoPull, "pipe1");
	
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
	//fazer ciclo para matar todos os clientes
	pthread_kill(t_server, SIGUSR1);
	pthread_join(t_server, &estado);
	remove(MEDIT_NAME_PIPE_PRINCI_V);
	exit(0);
}
