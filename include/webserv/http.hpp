# ifndef HTTP_HPP
# define HTTP_HPP

void acceptConnection(int listenFd, std::vector<Client*>& clients);

# endif