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
	servCli respostas;
	c=0;
	server server;
	//
	//Criação das pipes de comunicação com o servidor
	pipes_ini(&pid, &fd_abrirE, &nw, myPID, &myFifo);
	busca_ambiente(&server);
	if(argc==1){					
		sprintf(var_nome2,"Ucliente");
		var_nome=var_nome2;
		c=1;				
	}
	else
	{
		while((i = getopt(argc, argv, "u:p:n:")) != -1){
			switch(i){
				case 'u':
					var_nome = optarg;
					//Fazer a validação de users pelo lado  servidor
					envio.estado=0;
					strcpy(envio.nome, var_nome);

					if(write(myFifo, &envio, sizeof(cliServ))!=sizeof(cliServ)){
						fprintf(stderr,"[ERRO]Nao foi possivel verificar o utilizador");
					}

					if(read(fd_abrirE, &respostas, sizeof(cliServ))!=sizeof(cliServ)){
						fprintf(stderr,"[ERRO]Nao foi possivel verificar o utilizador");
					}

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
					if(c!=1){
						sprintf(var_nome2,"Ucliente");
						var_nome=var_nome2;	
						c=1;
					}
					break;
				case 'p':
					server.MEDIT_NUM_PIPES= atoi(optarg);
					if(c!=1){
						sprintf(var_nome2,"Ucliente");
						var_nome=var_nome2;	
						c=1;
					}					
					break;
				default:
					fprintf(stderr, "Opcao invalida!\n");
			}
		} 			
	}
	//Começa o editor
	if(c){			
		//Programa principal	
		documento(var_nome, &server);												
	}
	//Destroi o pipe que usa para comunicação com o server
	fim_pipe(myPID);
	exit(0);
}
