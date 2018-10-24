client:client.o
	cc -o client client.o
client.o:client.c
	cc -c client.c

server:server.o
	cc -o server server.o
server.o::server.c
	cc -c server.c

clean:
	rm server.o server client.o client


