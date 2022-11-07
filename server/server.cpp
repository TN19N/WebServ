#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <signal.h>
#include <string.h>
#include <iostream>

#define PORT "3490" // the port to listen to
#define BACKLOG 10   // pending connections queue max size

int main(void)
{
	struct addrinfo hints, *servInfo;
	struct sockaddr_storage *theirAddr; // connector's address information
	int rv;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	if ((rv = getaddrinfo(NULL, PORT, &hints, &servInfo)) != 0)
	{
		std::cerr << "getaddrinfo: " << gai_strerror(rv) << std::endl;
		return 0;
	}

	for (auto ptr = servInfo; ptr; ptr = ptr->ai_next)
	{
		std::cout << "-------------------------------" << std::endl;
		std::cout << "ai_flags: " << ptr->ai_flags << std::endl;
		std::cout << "ai_family: " << ptr->ai_family << std::endl;
		std::cout << "ai_socktype: " << ptr->ai_socktype << std::endl;
		std::cout << "ai_protocol: " << ptr->ai_protocol << std::endl;
		std::cout << "ai_addrlen: " << ptr->ai_addrlen << std::endl;
		if (ptr->ai_canonname)
			std::cout << "ai_canonname: " << ptr->ai_canonname << std::endl;
		else
			std::cout << "ai_canonname: (NULL)" << std::endl;
		if (ptr->ai_family == PF_INET)
		{
			struct add		
		}
		else if (ptr->ai_family == PF_INET6)
		{
			
		}
	}

	freeaddrinfo(servInfo);
}
