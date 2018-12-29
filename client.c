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
#include <pthread.h>
#include <signal.h>

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
	int i, c, myFifo, fd_abrirE, pid, nw, nr, fd_client_pipe, pid_server;
	cliServ envio;	//Estrutura Cliente-Servidor
	servCli respostas;	//Estrutura Servidor-Cliente
	server server;
	pthread_t t_client;	//Thread para estar sempre a ler
	
	busca_ambiente(&server);

	if(argc!=1){

		while((i = getopt(argc, argv, "u:")) != -1){

			switch(i){

				case 'u':
					var_nome = optarg;
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
	envio.resposta=0;
	respostas.muda=0;
	
	//Criação das pipes de comunicação com o servidor
	pipes_ini(&envio, &respostas, &fd_abrirE, &nw, myPID, &myFifo);	

	if((pthread_create(&t_client, NULL, le_pipe_Cli, (void *)&respostas))==-1){
		fprintf(stderr, "\nErro: criacao da thread principal do cliente\n");
	}

	if( (fd_abrirE=open(MEDIT_NAME_PIPE_PRINCI_V, O_WRONLY))==-1){
		fprintf(stderr, "Erro ao abir a pipe principal\n");
		exit(-1);
	}

	nw = write(fd_abrirE, &envio, sizeof(cliServ));
   	
	//ver erro desta parte pk de não passar o valor para fora

	while(respostas.muda==0){ 	//Espera pela resposta do servidor
	} 

	strcpy(envio.pipe_pull, respostas.fifo_serv);
	pid_server = respostas.pid;

	if(respostas.estado==0 && respostas.valID==1){
		envio.estado=1;
		documento(var_nome, &server, &respostas, &envio);
		envio.estado=2;
		envio.pid=getpid();
		nw = write(fd_abrirE, &envio, sizeof(cliServ));

	}
	else{
		printf("[ERRO] Esse cliente nao existe\n\n");
	}	

	//Destroi o pipe que usa para comunicação com o server
	fim_pipe(myPID);

	exit(0);
}
