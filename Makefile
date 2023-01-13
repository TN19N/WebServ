NAME = webserv
SRC_DIR = ./src
INCLUDE_DIR = ./include
CC = c++
CFLAGS = -Wall -Wextra -Werror -std=c++98 -fsanitize=address -g3
SRCS = $(SRC_DIR)/main.cpp
OBJS = $(SRCS:.cpp=.o)

.PHONY: all
all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

%.o: %.cpp
	$(CC) $(CFLAGS) -I $(INCLUDE_DIR) -c $< -o $@

.PHONY: clean
clean:
	rm -f $(OBJS)

.PHONY: fclean
fclean: clean
	rm -f $(NAME)

.PHONY: re
re: fclean all