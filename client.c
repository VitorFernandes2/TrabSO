#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <ncurses.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <fcntl.h>

/*------------------------------*/
/*            Imports           */
/*------------------------------*/
#include "fich_h/server_default.h"
#include "fich_h/medit_default.h"
#include "fich_h/client_default.h"
/*------------------------------*/

/*------------------------------*/
/*            Defines           */
/*------------------------------*/
#define PERM 0777
/*------------------------------*/



int main(int argc, char *argv[])
{
	char *var_nome=NULL, myPID[10];
	int i, c, myFifo, fd_abrirL, fd_abrirE, nw;
	c=0;
	server server;
	client cliente;

	if(argc==1){
        	fprintf(stderr,"%s %d: Falta de comandos\n",argv[0]);
		exit(-1);
	}
	
	cliente.pid=getpid();
	strcpy(cliente.nome, argv[2]);
	cliente.estado=0;
		
	if( (fd_abrirE=open(MEDIT_NAME_PIPE_PRINCI_V, O_WRONLY))==-1){
		fprintf(stderr, "\nErro ao abir a pipe de escrito\n");
		exit(-1);
	}
	
	nw = write(fd_abrirE, &cliente, sizeof(client));
		
	sprintf(myPID, "%d", cliente.pid);

	myFifo=mkfifo(myPID, PERM);
	
	if(myFifo==-1){
		if(errno==EEXIST){
			fprintf(stderr, "\nA fifo %s ja existe\n", myPID);
		}
		else {
			fprintf(stderr, "\nErro no mkfifo()\n");
			exit(-1);
		}
	}
	else {
		printf("\nFifo %s criado\n", myPID);
	}
	
	fd_abrirL=open(myPID, O_RDONLY);
	
	if(fd_abrirL==-1){
		fprintf(stderr, "\nErro: abertura do named pipe do cliente.\n");
	}
    	

	busca_ambiente(&server);

	while((i = getopt(argc, argv, "u:p:n:")) != -1){
		switch(i){
			case 'u':
				var_nome = optarg;
				if(verifica_user(server.MEDIT_FICHEIRO,var_nome,argv[0])==1){
						c=1;                    
				}
				else{
						fprintf(stderr,"%s: Esse utilizador nao existe\n",argv[0]);
						exit(-1);
				}   
				break;
			case 'n':
				server.MEDIT_NAME_PIPE_PRINCI=optarg;
				break;
			case 'p':
				server.MEDIT_NUM_PIPES= atoi(optarg);
				break;
			default:
				fprintf(stderr, "Opcao invalida!\n");
		}
	}    
	if(c)
		documento(var_nome, &server);
	exit(0);
}
