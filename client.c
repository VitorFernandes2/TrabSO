#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
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

int main(int argc, char *argv[])
{
	char *var_nome=NULL, myPID[10];
	char var_nome2[9];
	int i, c, myFifo, fd_abrirE, pid, nw;
	cliServ envio;	//Estrutura Cliente-Servidor
	servCli respostas;	//Estrutura Servidor-Cliente
	server server;

	busca_ambiente(&server);

	if(argc!=1){
		while((i = getopt(argc, argv, "u:p:n:")) != -1){
			switch(i){
				case 'u':
					var_nome = optarg;  
					break;
				case 'n':
					server.MEDIT_NAME_PIPE_PRINCI=optarg;
					break;
				case 'p':
					server.MEDIT_NUM_PIPES= atoi(optarg);				
					break;
				default:
					fprintf(stderr, "Opcao invalida!\n");
					exit(-1);
					break;
			}
		} 			
	}

	if(var_nome==NULL){
		printf("Insira o nome do utilizador: ");	
		scanf("%s",var_nome2);	
		var_nome=var_nome2;		
	}

	envio.estado=0;
	strcpy(envio.nome, var_nome);
	envio.pid=getpid();

	//Criação das pipes de comunicação com o servidor
	pipes_ini(&envio, &respostas, &fd_abrirE, &nw, myPID, &myFifo);	
	//Programa principal	
	documento(var_nome, &server);	
	//Destroi o pipe que usa para comunicação com o server
	fim_pipe(myPID);
												
	exit(0);
}
