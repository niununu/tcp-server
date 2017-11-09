.PHONY:all
all:server client

server:handle_require.c server.c
	gcc -o $@ $^ -lpthread

client:client.c
	gcc -o $@ $^

.PHONY:clean
clean:
	rm -rf client server
