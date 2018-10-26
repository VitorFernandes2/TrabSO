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

    initscr();
    clear();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    cabecalho(user,server);
    corpo();
    ch = getch();
    endwin();
}

void cabecalho(char *user,server *server){
    int posx; //variáveis de posição    
    posx=1;

    //INICIO
    mvaddch(0, 0, ACS_ULCORNER);
    while(posx < server->MEDIT_MAXCOLUMNS + 1){
        mvaddch(0, posx, ACS_HLINE);
        refresh(); 
        posx++;
    }
    mvaddch(0, posx, ACS_URCORNER);

    //MEIO
    mvaddch(1, posx, ACS_VLINE); 
    mvaddch(1, 0, ACS_VLINE);
    mvprintw(1,1,""); 
    printw("Bem-vindo, %s",user);
        
    //FUNDO
    mvaddch(2, 0, ACS_LLCORNER);
    posx = 1;
    while(posx < server->MEDIT_MAXCOLUMNS + 1){
        mvaddch(2, posx, ACS_HLINE);
        refresh(); 
        posx++;
    }   
    mvaddch(2, posx, ACS_LRCORNER);     
}

void corpo(){
    int posx; //variáveis de posição
    mvaddch(0, 0, ACS_ULCORNER);
    mvaddch(1, 0, ACS_VLINE);
    posx=1;
    while(posx < server->MEDIT_MAXCOLUMNS + 1){
        mvaddch(0, posx, ACS_HLINE);
        refresh(); 
        posx++;
    }
    mvaddch(0, posx, ACS_URCORNER);    
}