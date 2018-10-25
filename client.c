#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

/*------------------------------*/
/*            Imports           */
/*------------------------------*/
#include "fich_h/server_default.h"
/*------------------------------*/

int main(int argc, char *argv[])
{
    char *var_nome=NULL;
    int i;
    server server;
    if(argc!=2){
        fprintf(stderr,"%s: Esse comando nao existe\n",argv[0]);
        exit(-1);
    }
    if(busca_ambiente(argv[0],&server,argv[0])!=0){
        exit(-1);
    }
    else{
        if(verifica_user(server.MEDIT_FICHEIRO,argv[1],argv[0])==1){
            printf("Ola %s\n",argv[1]);
            //fazer interface gráfica
        }
        else{
            fprintf(stderr,"%s: Esse utilizador nao existe\n",argv[1]);
            exit(-1);
        }
    }
    exit(0);
}