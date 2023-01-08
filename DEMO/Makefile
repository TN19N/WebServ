all : server client

client :
	cd client && c++ -std=c++98 -o ../client.out client.cpp -D DEBUG

server :
	cd server && c++ -std=c++98 -o ../server.out server.cpp -D DEBUG

clean :
	rm -f server.out client.out

.PHONY : all clean server client