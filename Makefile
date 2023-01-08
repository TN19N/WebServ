NAME = webserv
BUILDDIR = ./build/objs
SRCSDIR = ./src
INCLUDES = ./include
SRCS = $(wildcard $(SRCSDIR)/*.cpp $(SRCSDIR)/*/*.cpp)
OBJS = $(patsubst $(SRCSDIR)/%.cpp, $(BUILDDIR)/%.o, $(SRCS)) 
CC = c++
CFLAGS = -Wall -Wextra -Werror -std=c++98
DEBUG = -fsanitize=address -g3

# colors
COLOUR_GREEN=\033[0;32m
COLOUR_RED=\033[0;31m
COLOUR_BLUE=\033[0;34m
COLOUR_END=\033[0m

# default
.PHONY : all
all : $(NAME)

$(NAME) : $(OBJS)
	@ echo "$(COLOUR_BLUE) Building ... $(COLOUR_END)"
	@ $(CC) $(CFLAGS) $(DEBUG) $(OBJS) -I $(INCLUDES) -o $(NAME)
	@ echo "$(COLOUR_GREEN) Done! $(COLOUR_END)"

$(BUILDDIR)/%.o : $(SRCSDIR)/%.cpp
	@ $(CC) $(CFLAGS) $(DEBUG) -I $(INCLUDES) -c $< -o $@

.PHONY : clean
clean :
	@ echo "$(COLOUR_RED) Cleaning ... $(COLOUR_END)"
	@ rm -rf $(BUILDDIR)/*.o $(BUILDDIR)/*/*.o
	@ echo "$(COLOUR_RED) Done! $(COLOUR_END)"

.PHONY : fclean
fclean : clean
	@ rm -rf $(NAME)

.PHONY : re
re : fclean all