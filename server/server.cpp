#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string>
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include "../includes/helper.hpp"

#define PORT "8080" // the port to listen to

int main(void)
{
	struct addrinfo hint, *servInfo;
	int rv;
	int sockfd;

	memset(&hint, 0, sizeof(hint));
	hint.ai_family = PF_INET;
	hint.ai_socktype = SOCK_STREAM;
	hint.ai_protocol = IPPROTO_TCP;
	hint.ai_flags = AI_ADDRCONFIG | AI_PASSIVE;

	if ((rv = getaddrinfo(NULL, PORT, &hint, &servInfo)) != 0)
	{
		std::cerr << "getaddrinfo: " << gai_strerror(rv) << std::endl;
		return 0;
	}

	for (auto ptr = servInfo; ptr; ptr = ptr->ai_next)
	{
		printInfo(ptr);
		if ((sockfd = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol)) == -1)
		{
			perror("server: socket()");
			continue;
		}
		printf("|sockfd: %d|\n", sockfd);
		if (bind(sockfd, ptr->ai_addr, ptr->ai_addrlen) == -1)
		{
			perror("server: bind()");
			close(sockfd);
			continue;
		}
		break;
	}

	//freeaddrinfo(servInfo);
	servInfo = NULL;
	system("leaks server.out");
}
