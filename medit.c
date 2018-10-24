#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "fich_h/medit-defaults.h"
#include "fich_h/server-defaults.h"
#include "fich_h/client-defaults.h"


int verificaUser(char *nomeFicheiro, char *username, char *exe){
	int i=0;
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
		i++;		
	}
	fclose(f);
	return 0;
}

int main(int argc, char**argv){
	char username[8];
	if(argc < 3 || argc > 4){
		fprintf(stderr,"%s: Comando inexistente\n",argv[0]);
	}
	else{
		if(strcmp(argv[1], "medit.db")==0){
			if((verificaUser("fich_db/medit.db", argv[2],argv[0]))==1)
				printf("Acesso permitido\n");
			else
				printf("Acesso negado\n");
		}
		else{
			if(strcmp(argv[1], "server")==0){
				if(strcmp(argv[2],"settings")==0){
					if(verificaUser("fich_db/admins.txt", argv[3], argv[0])==1){
						//fazer o comando de server
					}
				}				
			}
		}
	}	
	exit(0);
}
