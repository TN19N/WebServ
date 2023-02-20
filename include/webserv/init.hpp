# ifndef INIT_HPP
# define INIT_HPP

# include "webserv/context.hpp"

size_t init(const Context* configuration, std::vector<pollfd>& fds);

# endif