# Para executar este makefile é apenas necessário fazer
# make medit

HEADERS = server.c fich_c/server_default.c fich_c/medit_default.c

medit:
	cc -pthread -o server server.c fich_c/server_default1.c fich_c/medit_default.c
	cc -lncurses -pthread -o client fich_c/client_default.c client.c fich_c/server_default1.c fich_c/medit_default.c

# Para eliminar os executáveis é fazer apenas
# make delete

delete:
	-rm -f client
	-rm -f server
