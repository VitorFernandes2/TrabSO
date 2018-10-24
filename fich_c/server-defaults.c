#include "server-defaults.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h> 
#include <fcntl.h>

/*int settings(int *args){
    int pfd; //inteiro que vai guardar o indice do ficheiro
    if((pfd=open("../fich_db/settings.db",O_RDONLY)) == -1){ //verifica se o ficheiro existe, se existir copia o valor do descritor
        fprintf(stderr,"O ficheiro não existe.");
        return -1;
    }
    else{
        printf("O numero de linhas de cada documento e %d",MEDIT_MAXLINES);
        printf("O numero de colunas de cada documento e %d",MEDIT_MAXCOLUMNS);
        printf("O nome do ficheiro de base de dados dos usernames e %s",MEDIT_FICHEIRO);
        //Fazer printf da variável de nome da base de dados
        printf("O numero de colunas de cada documento e %d",MEDIT_MAXUSERS);
        //Fazer printf do número de named piped da interação
        //Fazer printf do nome da pipe principal   
    } 
    return 0;
}*/

int settings(){
    FILE *f;
    if((f=fopen("../fich_db/settings.db","r"))==NULL){
        printf("\nO numero de linhas de cada documento e %d",MEDIT_MAXLINES);
        printf("\nO numero de colunas de cada documento e %d",MEDIT_MAXCOLUMNS);
        printf("\nO nome do ficheiro de base de dados dos usernames e %s",MEDIT_FICHEIRO);
        printf("\nO numero de colunas de cada documento e %d",MEDIT_MAXUSERS);
        //Fazer printf do número de named piped da interação
        //Fazer printf do nome da pipe principal    
    }
    else{
        printf("Processo incoming...");
    }
    return 0;
}