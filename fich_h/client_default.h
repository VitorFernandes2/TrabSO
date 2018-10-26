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

#endif