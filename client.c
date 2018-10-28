#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ncurses.h>

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
    int i,c;
    c=0;
    server server;

    if(argc==1){
        fprintf(stderr,"%s: Falta de comandos\n",argv[0]);
        exit(-1);
    }

    busca_ambiente(&server);

    while((i = getopt(argc, argv, "u:p:n:")) != -1){
        switch(i){
            case 'u':
                var_nome = optarg;
                if(verifica_user(server.MEDIT_FICHEIRO,var_nome,argv[0])==1){
                    c=1;                    
                }
                else{
                    fprintf(stderr,"%s: Esse utilizador nao existe\n",argv[0]);
                    exit(-1);
                }   
                break;
            case 'n':
                server.MEDIT_NAME_PIPE_PRINCI=optarg;
                break;
            case 'p':
                server.MEDIT_NUM_PIPES= atoi(optarg);
                break;
            default:
				fprintf(stderr, "Opcao invalida!\n");
        }
    }    
    if(c)
        documento(var_nome, &server);
    exit(0);
}