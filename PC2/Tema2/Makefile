
build: server client

server: server.c comenzi.c
	gcc server.c comenzi.c -o server -g


client: client.c
	gcc client.c -o client -g

log:
	rm -f *.log

clean:
	rm -f server client *.log
