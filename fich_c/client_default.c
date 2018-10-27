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
            teclas(&posx, &posy, server); 
            mvprintw(posy, 5," ");
            posx=7;
            move_cursor(&posx, &posy); 
        }
    }while(ch != 27);

    adeus(user, server);
    
    endwin();
}

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
    int posx,posy,i; //variáveis de posição
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

void teclas(int *posx, int *posy, server *server){
    int ch;
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