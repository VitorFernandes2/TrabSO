#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

/*------------------------------*/
/*            Imports           */
/*------------------------------*/
#include "fich_h/server_default.h"
#include "fich_h/medit_default.h"
#include "fich_h/client_default.h"
/*------------------------------*/

int main(int argc, char *argv[])
{
    char *var_nome=NULL;
    int i;
    server server;
    if(argc==1){
        fprintf(stderr,"%s: Falta de comandos\n",argv[0]);
        exit(-1);
    }

    while((i = getopt(argc, argv, "u:")) != -1){
        switch(i){
            case 'u':
                var_nome = optarg;
                busca_ambiente(&server);
                if(verifica_user(server.MEDIT_FICHEIRO,var_nome,argv[0])==1){
                    printf("Ola %s\n",var_nome);
                    //fazer interface gráfica
                }
                else{
                    fprintf(stderr,"%s: Esse utilizador nao existe\n",argv[0]);
                    exit(-1);
                }
                break;
            case '?':
				fprintf(stderr, "Opcao invalida!\n");
        }
    } 
    exit(0);
}