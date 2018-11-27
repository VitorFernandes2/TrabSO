HEADERS = fich_c/server_default.c fich_c/medit_default.c


server:server.o
	gcc -o server server.o
server.o::server.c $(HEADERS)
	gcc -c server.c
