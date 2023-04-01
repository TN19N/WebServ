#  Compiler
CC := c++

# Directories
SRC_DIR := ./src
BUILD_DIR := ./build
TARGET_DIR := $(BUILD_DIR)/bin
OBJ_DIR := $(BUILD_DIR)/obj
INCLUDE_DIR := ./include
CONFIG_DIR := ./config
TEMPLATE_DIR := ./template

# The Target Binary Program
TARGET := $(TARGET_DIR)/webserv.exe
DEBUG_LOG_FILE := $(BUILD_DIR)/debug/debug.log

# extentions
SRC_EXT := cpp
OBJ_EXT := o

# Flags
CFLAGS := -Wall -Wextra -Werror
CFLAGS += -std=c++98

# Sources
# -------------------------------------------------------------
SRCS := $(SRC_DIR)/core/luncher.$(SRC_EXT) \
		$(SRC_DIR)/core/webserv.$(SRC_EXT) \
		$(SRC_DIR)/core/context.$(SRC_EXT) \
		$(SRC_DIR)/core/loadConfiguration.$(SRC_EXT) \
		$(SRC_DIR)/core/tools.$(SRC_EXT) \
		$(SRC_DIR)/http/client.$(SRC_EXT) \
		$(SRC_DIR)/http/connectionHandler.$(SRC_EXT) \
		$(SRC_DIR)/http/request.$(SRC_EXT) \
		$(SRC_DIR)/http/response.$(SRC_EXT) \
		$(SRC_DIR)/http/responseHandler.$(SRC_EXT) \
		$(SRC_DIR)/http/tools.$(SRC_EXT) \
		$(SRC_DIR)/http/requestHandler.$(SRC_EXT) \
		$(SRC_DIR)/http/baseParser.$(SRC_EXT) \
		$(SRC_DIR)/http/blockMatchAlgorithm.$(SRC_EXT) \
		$(SRC_DIR)/http/readBodyFromBuffer.$(SRC_EXT) \
		$(SRC_DIR)/http/getMethodHandler.$(SRC_EXT) \
		$(SRC_DIR)/http/postMethodHundler.$(SRC_EXT) \
		$(SRC_DIR)/http/deleteMethodHundler.$(SRC_EXT) \
		$(SRC_DIR)/http/IBase.$(SRC_EXT) \
		$(SRC_DIR)/http/cgiExecutor.$(SRC_EXT)

# -------------------------------------------------------------
OBJS := $(SRCS:$(SRC_DIR)/%.$(SRC_EXT)=$(OBJ_DIR)/%.$(OBJ_EXT))
TEMPLATES := $(wildcard $(TEMPLATE_DIR)/*)
CONFIGS := $(TEMPLATES:$(TEMPLATE_DIR)/%=$(CONFIG_DIR)/%)

# colors
RED := \033[0;31m
GREEN := \033[0;32m
END := \033[0m

.PHONY: all
all: debug

startCmp: 
	@ echo "$(GREEN)Compiling ...$(END)"
endCmp  : 
	@ echo "$(GREEN)Done!$(END)"

.PHONY: debug
debug: CFLAGS += -g3 -fsanitize=address
debug: CFLAGS += -D DEBUG
debug: startCmp $(TARGET) endCmp

.PHONY: release
release: startCmp $(TARGET) endCmp

.PHONY: run
run: generate debug
	@ ./$(TARGET) $(ARGS) > $(DEBUG_LOG_FILE)

.PHONY: generate
generate: $(CONFIGS)
	@ echo "$(GREEN)Generate config Done!$(END)"

$(TARGET) : $(OBJS)
	@ $(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

$(OBJ_DIR)/%.$(OBJ_EXT): $(SRC_DIR)/%.$(SRC_EXT)
	@ $(CC) $(CFLAGS) -c -o $@ $< -I $(INCLUDE_DIR)

$(CONFIG_DIR)/%: $(TEMPLATE_DIR)/%
	@ ./tools/envsubst.exe < $< > $@

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