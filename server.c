#include <sys/types.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include <sys/errno.h>
#include <pthread.h>
#include <signal.h>

#include "fich_h/server_default.h"
#include "fich_h/medit_default.h"


int main(int argc, char *argv[]){
	int numPipes=contaPipes(), tamArgc, fd_server_pipe, fd_pipe[numPipes], fifoPrincipal, fifoPull[numPipes], i, fase, e, k;
	char cline[50], lixo, *user,hostname[20], palavra1[20], palavra2[20], complementaPipe[100], pipeFinal[numPipes+ 4];  
	gethostname(hostname,20);
	pthread_t t_server, array_threads[numPipes];

	void *estado;

	user=getenv("USER");
	ambi();
	
	if(argc!=1){

		while((i = getopt(argc, argv, "f:p:n:")) != -1){

			switch(i){

				case 'f':
					muda_server(optarg);
					break;

				case 'n':
					printf("%s\n",optarg);

					for(i = 0; optarg[i] != '\0'; i++){

						if(optarg[i] < '0' || optarg[i] > '9')
						{
							
							fprintf(stderr, "Argumento do -n errado tem de meter apenas numeros inteiros!");
							exit(-1);

						}
							
					}
					mudaNPipes(atoi(optarg));
					break;

				case 'p':
					printf("%s\n",optarg);
					mudaMainPipe(optarg);				
					break;

				default:
					fprintf(stderr, "Opcao invalida!\n");
					exit(-1);
					break;

			}

		} 

	}

	inicio_matriz();

	pipe_ini(&fifoPrincipal, MEDIT_NAME_PIPE_PRINCI_V);
	pipe_ini(fifoPull, "pipe1");
	
	if((pthread_create(&t_server, NULL, le_pipe, (void *)&fd_server_pipe))==-1){
		fprintf(stderr, "\nErro: criacao da thread principal do server\n");
	}

	for(k=0; k<numPipes; k++){
		sprintf(complementaPipe, "%d", k);
		pipe_ini(&fifoPull[k], complementaPipe);
		if( (fd_pipe[k]=open(complementaPipe, O_RDWR))==-1){
			fprintf(stderr, "\nErro: leitura da pipe %d\n", i+1);
		}
		if((pthread_create(&array_threads[k], NULL, le_pipe1, (void *)&fd_pipe[k]))==-1){
			fprintf(stderr, "\nErro: criacao da thread no server de leitura da pipe %d\n", i+1);
		}
	}

	do{		

		strcpy(palavra1, "");
		strcpy(palavra2, "");
		strcpy(cline, "");

		limpa();
		printf("\nServidor:");
		printf("\nsettings");
		printf("\nload <filename>");
		printf("\nsave <filename>");
		printf("\nfree <linenumber>");
		printf("\nstatistics");
		printf("\nusers");
		printf("\ntext");
		printf("\nshutdown");

		printf("\n\n%s@%s:~$ ",user,hostname);
		fflush(stdin);
		fflush(stdout);
		fgets(cline, 50, stdin);

		i = 0;

		while(cline[i] == ' ' || cline[i] == '\t')
		{
			i++;
		}

		for(fase = 0, e = 0; cline[i] != '\0'; i++){
			
			if(fase == 0)
			{
				
				if (cline[i] != ' '  && cline[i] != '\t' && cline[i] != '\n') {
					palavra1[e] = cline[i];
					palavra1[e + 1] = '\0';
					e++;
				}
				else
				{
					fase++;
					e = 0;
				}

			}			
			else
			{

				if (cline[i] != ' ' && cline[i] != '\t' && cline[i] != '\n') {
					palavra2[e] = cline[i];
					e++;
				}
				else
				{
					palavra2[e] = '\0';
					e = 0;
				}

			}

			while(cline[i+1] == ' ' || cline[i+1] == '\t')
			{
				i++;
			}			

		}
		palavra1[19]='\0';
		palavra2[19]='\0';
		
		if(strcmp(palavra1, "settings")==0){
			settings();
			fflush(stdout);
		}
		else
			if(strcmp(palavra1, "load")==0){
				printf("load\n");
			}
			else
				if(strcmp(palavra1, "save")==0){
					printf("save\n");
				}
				else
					if(strcmp(palavra1, "free")==0){
						printf("free\n");
					}					
					else						
						if (strcmp(palavra1, "statistics")==0) {
							printf("statistics\n");
						}
						else
							if(strcmp(palavra1, "users")==0){
								mostraUsers();
								fflush(stdout);
							}					
							else
								if(strcmp(palavra1, "shutdown")!=0){			
									printf("%s: comando nao disponivel\n", palavra1);	
								}

	}while(strcmp(palavra1, "shutdown")!=0);

	pthread_kill(t_server, SIGUSR1);//Faz logout a todos os Users
	
	for(k=0; k < numPipes; k++){
		sprintf(complementaPipe, "%d", k);
		pthread_kill(array_threads[k], SIGUSR1);
		remove(complementaPipe);
	}

	remove("pipe1");
	remove(MEDIT_NAME_PIPE_PRINCI_V);
	liberta_users();
	liberta_matriz();

	exit(0);
}
