#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "fich_h/server_default.h"

void settings(){

	printf("\nNumero maximo de linhas: %d", atoi(getenv("MEDIT_MAXLINES")));
	printf("\nNumero maximo de colunas: %d", atoi(getenv("MEDIT_MAXCOLUMNS")));
	printf("\nNome do ficheiro da base de dados: %s", getenv("MEDIT_FICHEIRO"));
	printf("\nNumero maximo de users a editar: %d\n", atoi(getenv("MEDIT_MAXUSERS")));
	//falta os printfs relativos aos named pipes
	exit(0);
}

int main(int argc, char *argv[]){

	char cline[20]; 
	printf("\nServidor:");
	printf("\n1- settings");
	printf("\n2- load <filename>");
	printf("\n3- save <filename>");
	printf("\n4- free <linenumber>");
	printf("\n5- statistics");
	printf("\n6- users");
	printf("\n7- text");
	printf("\n8- shutdown");
	printf("\nCommand Line: ");
	scanf("%s", cline);
	if(strcmp(cline, "settings")==0){
		if(fork()){
			settings();
		}
		fflush(stdout);
	}
	exit(0);
}
