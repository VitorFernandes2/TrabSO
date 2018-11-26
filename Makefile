HEADERS = fich_c/server_default.c fich_c/client_default.c fich_c/medit_default.c

client:client.o
	gcc -o client client.o -lncurses
client.o::client.c $(HEADERS)
	gcc -c client.c

