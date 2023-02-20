# ifndef HTTP_HPP
# define HTTP_HPP

void acceptConnection(int listenFd, std::vector<pollfd>& fds, std::vector<Client*>& clients);

void receiveRequest(Client* client, std::vector<int>& fdsToClose);

# endif