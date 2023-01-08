all : server client

client :
	cd client && c++ -std=c++17 -o ../client.out client.cpp

server :
	cd server && c++ -std=c++17 -o ../server.out server.cpp

clean :
	rm -f server.out client.out

.PHONY : all clean server client