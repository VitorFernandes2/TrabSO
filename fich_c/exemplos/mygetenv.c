#include <stdio.h>
#include <stdlib.h>

int main(int argc, char**argv){
	char *var_nome=NULL;
	int i;

	if(argc<2){
		printf("Erro de sintaxe");
		return 0;
	}

	for(i=1; i<argc; i++){
		var_nome=getenv(argv[i]);
		if(var_nome==NULL){
			fprintf(stderr,"Variavel %s nao encontrada\n", argv[i]);
		}
		else{
			printf("%s\n", var_nome);
		}
	}
	return 0;
}

