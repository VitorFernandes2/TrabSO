#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv){
	char *valor = NULL;
	int estado = 0;
	int c;
	
	while((c = getopt(argc, argv, "nf:")) != -1){
		switch(c){
			case 'n':
				estado = 1;
				break;
			case 'f':
				valor = optarg;
				break;
			case '?':
				fprintf(stderr, "Opcao invalida!\n");

		}
	}
	printf("Estado = %d, Ficheiro = '%s' \n",estado,valor);
	
	exit(0);
}
