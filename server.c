#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "fich_h/server_default.h"
#include "fich_h/medit_default.h"

void settings(server *server){
	printf("\nNumero maximo de linhas: %d", server->MEDIT_MAXLINES);
	printf("\nNumero maximo de colunas: %d", server->MEDIT_MAXCOLUMNS);
	printf("\nNome do ficheiro da base de dados: %s", server->MEDIT_FICHEIRO);
	printf("\nNumero maximo de users a editar: %d", server->MEDIT_MAXUSERS);
	printf("\nNumero de pipes: %d", server->MEDIT_NUM_PIPES);
	printf("\nO nome da named pipe principal e: %s\n",server->MEDIT_NAME_PIPE_PRINCI);
}

int main(int argc, char *argv[]){
	server server;
	char cline[20]; 
	char lixo;
	char *user,hostname[20];
	int ret=0;
	user=getenv("USER");
	gethostname(hostname,20);
	do{
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
			ret=0;
			busca_ambiente(&server);
			settings(&server);
			fflush(stdout);
		}
		else
			ret=-1;			
	}while(ret==0);
	
	exit(0);
}
