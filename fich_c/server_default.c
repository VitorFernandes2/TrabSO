#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

/*------------------------------*/
/*            Imports           */
/*------------------------------*/
#include "../fich_h/server_default.h"
#include "../fich_h/medit_default.h"
/*------------------------------*/

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