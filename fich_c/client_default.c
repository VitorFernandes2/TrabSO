#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <string.h>
#include <unistd.h>

/*------------------------------*/
/*            Imports           */
/*------------------------------*/
#include "../fich_h/client_default.h"
#include "../fich_h/server_default.h"
#include "../fich_h/medit_default.h"
/*------------------------------*/

void documento(char *user, server *server){
    int nrow, ncol, posx, posy, oposx, oposy;
    int ch;
    char c;

    initscr();
    clear();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);

    //Design
    cabecalho(user,server);
    corpo(server);

    //Inicio da leitura do teclado
    c = mvinch(5, 7);
    mvprintw(5, 7, "%c", c); 
    do{
        ch = getch();

        //Fazer if's
    }while(ch != 27);

    adeus(user, server);
    
    endwin();
}

void cabecalho(char *user,server *server){
    int posx; //variáveis de posição    
    posx=1;

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
    printw("Bem-vindo, %s!",user);
        
    //FUNDO
    mvaddch(2, 0, ACS_LLCORNER);
    posx = 1;
    while(posx < server->MEDIT_MAXCOLUMNS + 7){
        mvaddch(2, posx, ACS_HLINE);
        refresh(); 
        posx++;
    }   
    mvaddch(2, posx, ACS_LRCORNER);     
}

void corpo(server *server){
    int posx,posy,i; //variáveis de posição
    i = 1;
    mvaddch(4, 0, ACS_ULCORNER);
    posx = 1;
    
    //INICIO
    while(posx < server->MEDIT_MAXCOLUMNS + 7){
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
    while(posx < server->MEDIT_MAXCOLUMNS + 7){
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