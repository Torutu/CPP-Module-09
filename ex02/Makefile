NAME = PmergeMe
CC = c++
CFLAGS = -Wall -Wextra -Werror -std=c++11

SRC = main.cpp PmergeMe.cpp
OBJ_DIR = obj
OBJ = $(SRC:%.cpp=$(OBJ_DIR)/%.o)

# color codes
RED = \033[31m
GREEN = \033[32m
MAGENTA = \033[35m
RESET = \033[0m

SHELL := /bin/bash

all: $(NAME)

$(OBJ_DIR):
	@ mkdir -p $(OBJ_DIR)

$(NAME): $(OBJ_DIR) $(OBJ)
	@ echo -ne "$(MAGENTA)Making $(NAME)"
	@ $(CC) $(CFLAGS) -o $(NAME) $(OBJ)
	@ echo -ne "\n$(GREEN)Done!$(RED)\n"

# Custom rule to ensure CC is used for compiling .cpp to .o and store in obj directory
$(OBJ_DIR)/%.o: %.cpp
	@ $(CC) $(CFLAGS) -c $< -o $@

clean:
	@ echo -e "$(RED)Removing $(OBJ_DIR) $(RESET)"
	@ rm -rf $(OBJ_DIR)

fclean: clean
	@ echo -e "$(RED)Removing $(NAME) $(RESET)"
	@ rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re