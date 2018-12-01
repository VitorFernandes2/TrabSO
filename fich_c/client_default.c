#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <sys/types.h>
#include <string.h>
#include <ncurses.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>


/*------------------------------*/

/*------------------------------*/
/*            Imports           */
/*------------------------------*/
#include "../fich_h/client_default.h"
/*------------------------------*/

void sig_handler(int signo)
{
    char myPID[10];
    if (signo == SIGUSR1){        
        endwin();
        printf("O servidor foi desligado\n");
        sprintf(myPID,"%d",getpid());
        fim_pipe(myPID);
    }    
    exit(0);
}

void * le_pipe_Cli (void * arg){
	int fd, myPID, nr;
	char myPipe[10];
	servCli *recebe;
	recebe = (servCli*) arg;
	
    	myPID=getpid();
	sprintf(myPipe, "%d", myPID);

	if( (fd=open(myPipe, O_RDWR))==-1){
		fprintf(stderr, "\nErro ao abir a pipe de leitura do cliente\n");
		exit(-1);
	}
	
	while((nr = read(fd, recebe, sizeof(servCli)))>0){
   	}
}

void pipes_ini(cliServ *client, servCli *serv, int *fd_abrirE, int *nw, char *myPID, int *myFifo){

    sprintf(myPID,"%d",client->pid);

    *myFifo=mkfifo(myPID, PERM);    //Criação da pipe de leitura do cliente
    
    if(*myFifo==-1){
        if(errno!=EEXIST){
            fprintf(stderr, "\nErro no mkfifo()\n");
            exit(-1);
        }
    }
    
}

void fim_pipe(char *myPID){
    unlink(myPID);
    remove(myPID);
}

void documento(char *user, server *server, servCli *respostas, cliServ *envio){
    int nrow, ncol, posx, posy, oposx, oposy;
    char c;
    int ch, token; 
    //A var. token serve para alternar entre modo edição de linha
    //e modo navegação de texto

    initscr();
    clear();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);

    //Design
    cabecalho(user,server);
    corpo(server);

    //Inicio da leitura do teclado 
    c = mvinch(posy, posx);
    posx = 7;
    posy = 5;
    move_cursor(&posx, &posy);

    do{
        signal(SIGUSR1,sig_handler);

        ch = getch();
        posx = 7;

        if(ch == KEY_DOWN){
            if(posy < server->MEDIT_MAXLINES + 4){
                posy++;
                move_cursor(&posx, &posy);
            }                
        }
        if(ch == KEY_UP){
            if(posy > 5){
                posy--;
                move_cursor(&posx, &posy);
            }
        }
        if(ch==10){ //ENTER  
            mvprintw(posy, 5,">");
            move_cursor(&posx, &posy);          
            teclas(&posx, &posy, server, respostas, envio); 
            mvprintw(posy, 5," ");
            posx=7;
            move_cursor(&posx, &posy); 
        }         
    }while(ch != 27);

    adeus(user, server);
    
    endwin();
}

//Funções de Design

void cabecalho(char *user,server *server){
    int posx; //variáveis de posição    
    posx=1;

    //INICIO
    mvaddch(0, 0, ACS_ULCORNER);
    while(posx < server->MEDIT_MAXCOLUMNS + 8){
        mvaddch(0, posx, ACS_HLINE);
        refresh(); 
        posx++;
    }
    mvaddch(0, posx, ACS_URCORNER);

    //MEIO
    mvaddch(1, posx, ACS_VLINE); 
    mvaddch(1, 0, ACS_VLINE);
    mvprintw(1, 1,""); 
    printw("Bem-vindo, %s!",user);
        
    //FUNDO
    mvaddch(2, 0, ACS_LLCORNER);
    posx = 1;
    while(posx < server->MEDIT_MAXCOLUMNS + 8){
        mvaddch(2, posx, ACS_HLINE);
        refresh(); 
        posx++;
    }   
    mvaddch(2, posx, ACS_LRCORNER);     
}

void corpo(server *server){
    int posx,posy,i,e; //variáveis de posição
    i = 1;
    mvaddch(4, 0, ACS_ULCORNER);
    posx = 1;
    
    //INICIO
    while(posx < server->MEDIT_MAXCOLUMNS + 8){
        mvaddch(4, posx, ACS_HLINE);
        refresh(); 
        posx++;
    }
    mvaddch(4, posx, ACS_URCORNER); 
    mvaddch(4, 4, ACS_TTEE);
    mvaddch(4, 6, ACS_TTEE);

    //BASE
    posy = 5;
    while(posy < server->MEDIT_MAXLINES + 5){
        if(i < 10)
            mvprintw(posy, 3, "%d", i);
        else
            if(i < 100)
                mvprintw(posy, 2, "%d", i);
            else
                if(i < 1000)
                    mvprintw(posy, 1, "%d", i);
        mvaddch(posy, 4, ACS_VLINE);
        mvaddch(posy, 6, ACS_VLINE);
        mvaddch(posy, 0, ACS_VLINE);
        mvaddch(posy, posx, ACS_VLINE);
        refresh(); 
        i++;
        posy++;
    } 

    //FIM
    posx=0;
    while(posx < server->MEDIT_MAXCOLUMNS + 8){
        mvaddch(posy, posx, ACS_HLINE);
        refresh(); 
        posx++;
    }
    mvaddch(posy, 0, ACS_LLCORNER);
    mvaddch(posy, posx, ACS_LRCORNER); 
    mvaddch(posy, 6, ACS_BTEE);
    mvaddch(posy, 4, ACS_BTEE);
}

void adeus(char *user, server *server){    
    int posx; //variáveis de posição    
    posx=1;
    
    clear();
    
    //INICIO
    mvaddch(0, 0, ACS_ULCORNER);
    while(posx < server->MEDIT_MAXCOLUMNS + 7){
        mvaddch(0, posx, ACS_HLINE);
        refresh(); 
        posx++;
    }
    mvaddch(0, posx, ACS_URCORNER);

    //MEIO
    mvaddch(1, posx, ACS_VLINE); 
    mvaddch(1, 0, ACS_VLINE);
    mvprintw(1, 1,""); 
    printw("Adeus, %s!",user);
        
    //FUNDO
    mvaddch(2, 0, ACS_LLCORNER);
    posx = 1;
    while(posx < server->MEDIT_MAXCOLUMNS + 7){
        mvaddch(2, posx, ACS_HLINE);
        refresh(); 
        posx++;
    }   
    mvaddch(2, posx, ACS_LRCORNER); 
    refresh();
    usleep(1000000);
}

//Função para ler as teclas

void teclas(int *posx, int *posy, server *server, servCli *respostas, cliServ *envio){
    int ch, i, fd, ne;
    char *linha2, *linha;
    linha=malloc(server->MEDIT_MAXCOLUMNS*sizeof(char));
    linha2=malloc(server->MEDIT_MAXCOLUMNS*sizeof(char));
    i=0;
    
    for(i=0; i<server->MEDIT_MAXCOLUMNS; i++){
        *(linha + i) = ' ';
    }
    apanha_linha(posy, linha, server);
    do{
        ch = getch();
        if(ch == KEY_LEFT){
            if((*posx) > 7){
                (*posx)--;
                move_cursor(posx, posy);
            }
        }
        if(ch == KEY_RIGHT){
            if((*posx) < server->MEDIT_MAXCOLUMNS + 6){
                (*posx)++;
                move_cursor(posx, posy);
            }
        }
        if(ch == KEY_DC){
            delete_linha(posx,posy,server);
        }
        if(ch == KEY_BACKSPACE){
            backspace(posx,posy,server);
        }
        if(ch >= 32 && ch <= 126){ //Entrada de números,letras e alguns char especiais            
            if(valida_texto(posx,posy,server)==1){
                mvprintw((*posy), (*posx), "%c", ch);                
                if((*posx)< server->MEDIT_MAXCOLUMNS + 6){
                    (*posx)++;
                } 
            }                        
        }
        if(ch==27){ //ESC
            escape(posy, linha, server);
        }
        else
            if(ch!=10){
                envio->linha = (*posy) - 4;
                apanha_linha(posy, linha2, server);
                strcpy(envio->Frase, linha2);

                if( (fd=open(respostas->fifo_serv, O_WRONLY))==-1){                    
                    endwin();
                    fprintf(stderr, "\nErro ao abir a pipe de leitura do cliente\n");
                    exit(-1);
                }

                ne = write(fd, envio, sizeof(cliServ));
            }
    }while(ch != 27 && ch != 10);
}

void move_cursor(int *posx, int *posy){
    char c;
    c = mvinch((*posy), (*posx));
}

void delete_linha(int *posx, int *posy, server *server){
    int i;
    char c;
    for(i = (*posx) + 1; i < server->MEDIT_MAXCOLUMNS + 6; i++){
        c = mvinch((*posy), i + 1);
        mvprintw((*posy), i, "%c", c);
    }
    mvprintw((*posy), i, " "); 
    move_cursor(posx, posy);
}

void backspace(int *posx, int *posy, server *server){
    int i;
    char c;
    if((*posx)>7){
        c = mvinch((*posy),(*posx));
        mvprintw((*posy), (*posx) - 1, "%c", c);
        for(i = (*posx); i < server->MEDIT_MAXCOLUMNS + 6; i++){
            c = mvinch((*posy), i + 1);
            mvprintw((*posy), i, "%c", c);
        }
        mvprintw((*posy), i, " ");
        (*posx)--;
        move_cursor(posx, posy);
    }    
}

int valida_texto(int *posx, int *posy, server *server){
    int i;
    char c;

    c = mvinch((*posy),server->MEDIT_MAXCOLUMNS + 6);
    if(c == ' '){
        for(i = server->MEDIT_MAXCOLUMNS + 6; i > (*posx); i--){
            c = mvinch((*posy), i-1);
            mvprintw((*posy), i, "%c", c);
        }
        move_cursor(posx, posy);
        return 1;
    }
    else
        move_cursor(posx, posy);
    return 0;
}

void escape(int *posy, char *linha, server *server){
    int i, e;
    char c, *copia;
    copia=linha;
    e=0;
    for(i=7; i<server->MEDIT_MAXCOLUMNS + 7; i++){
        mvprintw((*posy), i, "%c", copia[e]);
        e++;
    }
}

void apanha_linha(int *posy, char *linha, server *server){
    int i , e;
    char c, *copia;
    copia = linha;  
    e=0;   
    for (i = 7; i < server->MEDIT_MAXCOLUMNS + 7; i++){        
        c = mvinch((*posy), i);
        copia[e]=c;
        e++; 
    }
    c = mvinch((*posy), 7);
}
