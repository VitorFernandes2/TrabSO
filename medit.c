#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "medit-defaults.h"
#include "server-defaults.h"
#include "client-defaults.h"


int verificaUser(char*nomeFicheiro, char *username){

	FILE *f;
	char auxiliar[8];
	if((f=fopen(nomeFicheiro,"r"))==NULL){
		fprintf(stderr, "ERRO: erro ao abrir o ficheiro\n");
		return -1;
	}
	while(fscanf(f,"%s", auxiliar)==1){
		if(strcmp(auxiliar, username)==0){
			printf("\nExiste nome\n");
		}
	}
	fclose(f);
	return 0;
}



int main(int argc, char**argv){
	
	if(argc!=3){
		printf("\nErro de sintaxe: executavel nome_ficheiro username\n");
		exit(0);
	}
	char username[8];
	verificaUser(argv[1], argv[2]); 	
	exit(0);
	//ola
}


	

