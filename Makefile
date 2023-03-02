#  Compiler
CC := c++

# Directories
SRC_DIR := ./src
BUILD_DIR := ./build
TARGET_DIR := $(BUILD_DIR)/bin
OBJ_DIR := $(BUILD_DIR)/obj
INCLUDE_DIR := ./include

# The Target Binary Program
TARGET := $(TARGET_DIR)/webserv.exe

# extentions
SRC_EXT := cpp
OBJ_EXT := o

# Flags
CFLAGS := #-Wall -Wextra -Werror -std=c++98

# Sources
# -------------------------------------------------------------
SRCS := $(SRC_DIR)/main.$(SRC_EXT) \
		$(SRC_DIR)/core/webserv.$(SRC_EXT) \
		$(SRC_DIR)/core/context.$(SRC_EXT) \
		$(SRC_DIR)/core/loadConfiguration.$(SRC_EXT) \
		$(SRC_DIR)/core/init.$(SRC_EXT) \
		$(SRC_DIR)/http/client.$(SRC_EXT) \
		$(SRC_DIR)/http/acceptConnection.$(SRC_EXT) \
		$(SRC_DIR)/http/blockMatchAlgorithm.$(SRC_EXT) \
		$(SRC_DIR)/http/requestHandler.$(SRC_EXT) \
		$(SRC_DIR)/http/statusCodeHandler.$(SRC_EXT) \
		$(SRC_DIR)/http/responseHandler.$(SRC_EXT) \
		$(SRC_DIR)/http/tools.$(SRC_EXT) \
		$(SRC_DIR)/request/request_parser.$(SRC_EXT) \
		$(SRC_DIR)/request/request_handler.$(SRC_EXT) \
		$(SRC_DIR)/request/get_method_handler.$(SRC_EXT)

# -------------------------------------------------------------
OBJS := $(SRCS:$(SRC_DIR)/%.$(SRC_EXT)=$(OBJ_DIR)/%.$(OBJ_EXT))

# colors
RED := \033[0;31m
GREEN := \033[0;32m
END := \033[0m

.PHONY: all
all: debug

.PHONY: debug
debug: CFLAGS += -g3 -fsanitize=address -D DEBUG_GET_LOCATION
debug: $(TARGET)

.PHONY: release
release: $(TARGET)

.PHONY: run
run: debug
	@ ./$(TARGET) $(ARGS)

$(TARGET) : $(OBJS)
	@ echo "$(GREEN)Compiling ...$(END)"
	@ $(CC) $(CFLAGS) -o $(TARGET) $(OBJS)
	@ echo "$(GREEN)Done!$(END)"

$(OBJ_DIR)/%.$(OBJ_EXT): $(SRC_DIR)/%.$(SRC_EXT)
	@ $(CC) $(CFLAGS) -c -o $@ $< -I $(INCLUDE_DIR)

.PHONY: clean
clean:
	@ echo "$(RED)Cleaning ...$(END)"
	@ rm -rf $(OBJS)
	@ echo "$(RED)Done!$(END)"

.PHONY: fclean
fclean: clean
	@ rm -rf $(TARGET)

.PHONY: re
re: fclean all
