all: server client

server:
	cd server && c++ -std=c++98 server.cpp -o ../server.out

client:
	cd client && c++ -std=c++98 client.cpp -o ../client.out

.PHONY: all server client