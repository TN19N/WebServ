CFLAGS = -Wall -Wextra -Werror -std=c++98

all: server client

server:
	cd Server && c++ $(CFLAGS) server.cpp -o ../server.out

client:
	cd Client && c++ $(CFLAGS) client.cpp -o ../client.out

clean:
	rm -rf server.out client.out

re: clean all

.PHONY: all server client clean