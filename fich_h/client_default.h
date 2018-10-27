#ifndef _CLIENT_DEFAULT_H
#define _CLIENT_DEFAULT_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ncurses.h>

#include "medit_default.h"
#include "server_default.h"

void documento(char *user, server *server);
void cabecalho(char *user,server *server);
void corpo(server *server);
void adeus(char *user, server *server);
void teclas(int *posx, int *posy, server *server);
void move_cursor(int *posx, int *posy);
void delete_linha(int *posx, int *posy, server *server);
void backspace(int *posx, int *posy, server *server);
int valida_texto(int *posx, int *posy, server *server);

#endif