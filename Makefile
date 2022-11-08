all: server client

server:
	cd server && c++ -std=c++11 server.cpp -o ../server.out

client:
	cd client && c++ -std=c++11 client.cpp -o ../client.out

.PHONY: all server client