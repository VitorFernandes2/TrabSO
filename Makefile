# Para executar este makefile é apenas necessário fazer
# make -r server client

HEADERS = server.c fich_c/server_default.c fich_c/medit_default.c
HEADERS2 = client.c fich_c/client_default.c fich_c/server_default.c fich_c/medit_default.c

server: $(HEADERS)
	cc -pthread -o server server.c fich_c/server_default.c fich_c/medit_default.c

client: $(HEADERS2)
	cc -lncurses -pthread -o client fich_c/client_default.c client.c fich_c/server_default.c fich_c/medit_default.c