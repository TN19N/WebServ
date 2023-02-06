SRC_DIR = ./src
BUILD_DIR = ./build
OBJ_DIR = $(BUILD_DIR)/obj
BIN_DIR = $(BUILD_DIR)/bin
INCLUDE_DIR = ./include

NAME = $(BIN_DIR)/webserv.exe

CC = c++
CFLAGS = -Wall -Wextra -Werror -std=c++98

# ----------------------------------------------
SRC_FILES = $(SRC_DIR)/core/webserv.cpp
# ----------------------------------------------

OBJ_FILES = $(SRC_FILES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

# Colors
RED = \033[0;31m
GREEN = \033[0;32m
END = \033[0m

.PHONY: all
all: debug

.PHONY: debug
debug: CFLAGS += -g3 -fsanitize=address
debug: $(NAME)

.PHONY: release
release: $(NAME)

$(NAME): $(OBJ_FILES)
	@ echo "$(GREEN)Compiling...$(END)"
	@ $(CC) $(CFLAGS) -o $(NAME) $(OBJ_FILES) -I $(INCLUDE_DIR)
	@ echo "$(GREEN)Done!$(END)"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@ $(CC) $(CFLAGS) -c $< -o $@ -I $(INCLUDE_DIR)

.PHONY: clean
clean:
	@ echo "$(RED)Cleaning...$(END)"
	@ rm -rf $(OBJ_FILES)
	@ echo "$(RED)Done!$(END)"

.PHONY: fclean
fclean: clean
	@ rm -rf $(NAME)

.PHONY: re
re: fclean all