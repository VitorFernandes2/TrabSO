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

int xpos, ypos;
char **matriz, **matrizP;
server *server1;

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

void valida_textoClient(int x, int y, char c1)
{

    int i;
    char c;

    if(matriz[y][server1->MEDIT_MAXCOLUMNS - 1] == ' ')
    {

        for(i = server1->MEDIT_MAXCOLUMNS - 1; i > x; i--)
        {		

            matriz[y][i] = matriz[y][i - 1];
            mvprintw(y + 5, i + 7, "%c", matriz[y][i]);
            move_cursor(&xpos, &ypos);
            refresh();
            
        }
        
        matriz[y][x] = c1;
        mvprintw(y + 5, x + 7, "%c", matriz[y][x]);
        move_cursor(&xpos, &ypos);
        refresh();

    }
        
}

void deleteClient(int x, int y)
{

	int i;

	for(i = x + 1; i < server1->MEDIT_MAXCOLUMNS - 1; i++)
	{

        matriz[y][i] = matriz[y][i + 1];
        mvprintw(y + 5, i + 7, "%c", matriz[y][i]);
        move_cursor(&xpos, &ypos);
        refresh();		
		
	}

    matriz[y][i] = ' ';
    mvprintw(y + 5, i + 7, "%c", matriz[y][i]);
    move_cursor(&xpos, &ypos);
    refresh();

}

void backspaceClient(int x, int y)
{

    int i;
    
	if(x > -1){
        
        for(i = x; i < server1->MEDIT_MAXCOLUMNS - 1; i++)
        {

            matriz[y][i] = matriz[y][i + 1];
            mvprintw(y + 5, i + 7, "%c", matriz[y][i]);
            move_cursor(&xpos, &ypos);
            refresh();		
            
        }

        matriz[y][i] = ' ';
        mvprintw(y + 5, i + 7, "%c", matriz[y][i]);
        move_cursor(&xpos, &ypos);
        refresh();

    }

}

void escClient(int y)
{

    for(int i = 0; i < server1->MEDIT_MAXCOLUMNS - 1; i++)
    {

        mvprintw(y + 5, i + 7, "%c", matriz[y][i]);
        move_cursor(&xpos, &ypos);
        refresh();

    }

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
        if(recebe->estado == 4)
        {
            //caso seja del
            if(recebe->c == 74){
                deleteClient(recebe->coluna, recebe->linha);
            }
                

            //Caso seja caracter
            else
                if(recebe->c >= 32 && recebe->c <= 126){
                    valida_textoClient(recebe->coluna, recebe->linha, recebe->c);
                }
                    

            //Caso seja backspace
            else
                if(recebe->c == 7)
                {
                    backspaceClient(recebe->coluna, recebe->linha);
                }
                    

            //caso seja esc
            else
                if(recebe->c == 27){
                    matriz[recebe->linha] = matrizP[recebe->linha];
                    escClient(recebe->linha);
                }

            //caso seja enter
            else
                if(recebe->c == 10){
                    matrizP[recebe->linha] = matriz[recebe->linha];
                }

        }

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

    server1 = server;

    matriz = (char **) malloc(server->MEDIT_MAXLINES * sizeof(char *));
	matrizP = (char **) malloc(server->MEDIT_MAXLINES * sizeof(char *));

	for(int i = 0; i < server->MEDIT_MAXLINES; i++)
	{
		
		matriz[i] = (char *) malloc(server->MEDIT_MAXCOLUMNS * sizeof(char));
		matrizP[i] = (char *) malloc(server->MEDIT_MAXCOLUMNS * sizeof(char));

	}
	
	for(int i = 0; i < server->MEDIT_MAXLINES; i++)
	{
		
		for(int j = 0; j < server->MEDIT_MAXCOLUMNS; j++)
		{
			matriz[i][j] = ' ';
			matrizP[i][j] = ' ';
		}

	}

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
        xpos = posx;
        ypos = posy;
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
    int ch, i, fd, fd2, ne, nl, myPID;
    char *linha2, *linha, c, myPipe[10];
    linha=malloc(server->MEDIT_MAXCOLUMNS*sizeof(char));
    linha2=malloc(server->MEDIT_MAXCOLUMNS*sizeof(char));
    i=0;

    //Mandar Aqui uma pergunta ao servidor para saber se a linha está ocupada
    envio->linha = (*posy) - 5;
    envio->pid = getpid();
    envio->estado = 2;
    envio->caracter = '\0';

    if( (fd=open(envio->pipe_pull, O_WRONLY))==-1){                    
        endwin();
        fprintf(stderr, "\nErro ao abir a pipe de escrita do cliente\n");
        exit(-1);
    }

    ne = write(fd, envio, sizeof(cliServ));
    close(fd);

    myPID=getpid();
    sprintf(myPipe, "%d", myPID);

    if( (fd2=open(myPipe, O_RDONLY))==-1){                    
        endwin();
        fprintf(stderr, "\nErro ao abir a pipe de leitura do cliente\n");
        exit(-1);
    }
    
    respostas->muda=0;
    while(respostas->muda==0){ 	//Espera pela resposta do servidor
    }
    
    close(fd2);  
    
    if(respostas->estado == 2)
        return;

    for(i=0; i<server->MEDIT_MAXCOLUMNS; i++){
        *(linha + i) = ' ';
    }
    apanha_linha(posy, linha, server);
    do{
        xpos = (*posx);
        ypos = (*posy);
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

            envio->linha = (*posy) - 5;
            envio->coluna = (*posx) - 7;
            envio->caracter = ch;
            
            c = mvinch((*posy),(*posx));

            if( (fd=open(envio->pipe_pull, O_WRONLY))==-1){                    
                endwin();
                fprintf(stderr, "\nErro ao abir a pipe de escrita do cliente\n");
                exit(-1);
            }

            ne = write(fd, envio, sizeof(cliServ));
            close(fd);
        }
        if(ch == KEY_BACKSPACE){           

            envio->linha = (*posy) - 5;
            envio->coluna = (*posx) - 8;
            envio->caracter = ch;
            
            c = mvinch((*posy),(*posx));

            if( (fd=open(envio->pipe_pull, O_WRONLY))==-1){                    
                endwin();
                fprintf(stderr, "\nErro ao abir a pipe de escrita do cliente\n");
                exit(-1);
            }

            ne = write(fd, envio, sizeof(cliServ));
            close(fd);

            backspace(posx,posy,server);

        }
        if(ch >= 32 && ch <= 126){ //Entrada de números,letras e alguns char especiais  

            if(valida_texto(posx,posy,server)==1){

                mvprintw((*posy), (*posx), "%c", ch);    

                if((*posx)< server->MEDIT_MAXCOLUMNS + 6){
                    (*posx)++;
                }
                
                envio->linha = (*posy) - 5;
                envio->coluna = (*posx) - 8;
                envio->caracter = ch;

                if( (fd=open(envio->pipe_pull, O_WRONLY))==-1){                    
                    endwin();
                    fprintf(stderr, "\nErro ao abir a pipe de escrita do cliente\n");
                    exit(-1);
                }

                ne = write(fd, envio, sizeof(cliServ));
                close(fd);

            } 

        }
        if(ch==27){ //ESC
            escape(posy, linha, server);

            envio->linha = (*posy) - 5;
            envio->coluna = (*posx) - 8;
            envio->caracter = ch;

            if( (fd=open(envio->pipe_pull, O_WRONLY))==-1){                    
                endwin();
                fprintf(stderr, "\nErro ao abir a pipe de escrita do cliente\n");
                exit(-1);
            }

            ne = write(fd, envio, sizeof(cliServ));
            close(fd);
        }
        else
            if(ch==10){

                envio->linha = (*posy) - 5;
                envio->coluna = (*posx) - 8;
                envio->caracter = '\n';
                
                c = mvinch((*posy),(*posx));

                if( (fd=open(envio->pipe_pull, O_WRONLY))==-1){                    
                    endwin();
                    fprintf(stderr, "\nErro ao abir a pipe de escrita do cliente\n");
                    exit(-1);
                }

                ne = write(fd, envio, sizeof(cliServ));
                close(fd);

                myPID=getpid();
	            sprintf(myPipe, "%d", myPID);

                if( (fd2=open(myPipe, O_RDONLY))==-1){                    
                    endwin();
                    fprintf(stderr, "\nErro ao abir a pipe de leitura do cliente\n");
                    exit(-1);
                }
                
                respostas->muda=0;
                while(respostas->muda==0){ 	//Espera pela resposta do servidor
                }
                if(respostas->perm==0){
                    ch='a';
                }  
                else{
                    ch=10;
                }
                close(fd2);              
            }
    }while(ch != 27 && ch != 10);
    apanha_linha(posy, matriz[(*posy) - 5], server);
    apanha_linha(posy, matrizP[(*posy) - 5], server);
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
