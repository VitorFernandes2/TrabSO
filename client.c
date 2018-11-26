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
	char *var_nome=NULL;
	int i,c, myPID, fd1, myFifo;
	c=0;
	myPID=getpid();
	server server;
	fd_set fd_leitura;
	FD_ZERO(&fd_leitura);

	if(argc==1){
        	fprintf(stderr,"%s %d: Falta de comandos\n",argv[0]);
		exit(-1);
	}

	myFifo=mkfifo("ola.fifo", PERM);


	if(myFifo==-1){
		if(errno==EEXIST){
			fprintf(stderr, "\nA fifo %s ja existe\n", myFifo);
		}
		else {
			fprintf(stderr, "\nErro no mkfifo()\n");
		}
	}
	else {
		printf("\nFifo %s criado\n", myFifo);
	}

	fd1=open("ola.fifo", O_RDONLY);
	
	if(fd1==-1){
		fprintf(stderr, "\nErro: abertura do named pipe do cliente.\n");
	}
	
	
	FD_SET(fd1, &fd_leitura);
    	

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
