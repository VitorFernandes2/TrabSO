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

void busca_ambiente(server *server){ //função que vai buscar as variáveis de ambiente se existirem
    if(getenv("MEDIT_FICHEIRO")==NULL){
        server->MEDIT_FICHEIRO=MEDIT_FICHEIRO_V;
    }
    else{
        server->MEDIT_FICHEIRO=getenv("MEDIT_FICHEIRO");
    }

    if(getenv("MEDIT_NAME_PIPE_PRINCI")==NULL){
        server->MEDIT_NAME_PIPE_PRINCI=MEDIT_NAME_PIPE_PRINCI_V;
    }
    else{
        server->MEDIT_NAME_PIPE_PRINCI=getenv("MEDIT_NAME_PIPE_PRINCI");
    }

    if(getenv("MEDIT_MAXUSERS")==NULL){
        server->MEDIT_MAXUSERS=MEDIT_MAXUSERS_V;
    }
    else{
        server->MEDIT_MAXUSERS=atoi(getenv("MEDIT_MAXUSERS"));
    }
    
    if(getenv("MEDIT_MAXLINES")==NULL){
        server->MEDIT_MAXLINES=MEDIT_MAXLINES_V;
    }
    else{
        if(atoi(getenv("MEDIT_MAXLINES"))<1000)        
            server->MEDIT_MAXLINES=atoi(getenv("MEDIT_MAXLINES"));
        else
            server->MEDIT_MAXLINES=999;
    }

    if(getenv("MEDIT_MAXCOLUMNS")==NULL){
        server->MEDIT_MAXCOLUMNS=MEDIT_MAXCOLUMNS_V;
    }
    else{     
        server->MEDIT_MAXCOLUMNS=atoi(getenv("MEDIT_MAXCOLUMNS"));
    }

    if(getenv("MEDIT_NUM_PIPES")==NULL){
        server->MEDIT_NUM_PIPES=MEDIT_NUM_PIPES_V;
    }
    else{
        server->MEDIT_NUM_PIPES=atoi(getenv("MEDIT_NUM_PIPES"));
    }
}

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

void kill_thread(){
	pthread_exit(0);
}

void * le_pipe (void * arg){
	int fd, nr, pid, nw, fd_abrirE;
	fd= *(int*) arg;
	char pipePID[10];
    cliServ recebe;
	servCli resposta;

	signal(SIGUSR1, kill_thread);

	if( (fd=open(MEDIT_NAME_PIPE_PRINCI_V, O_RDWR))==-1){
		fprintf(stderr, "\nErro ao abir a pipe de leitura\n");
	}
	
	while((nr = read(fd, &recebe, sizeof(cliServ)))>0){		
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
