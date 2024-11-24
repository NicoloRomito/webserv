NAME = webserver

SRC_DIR = src/

FILES = main.cpp init/init.cpp utils/errorMsg.cpp 

SRC = $(addprefix $(SRC_DIR), $(FILES))

INCLUDES = MutantStack.hpp MutantStack.tpp

CPP = c++

CFLAGS = -Wall -Wextra -Werror -g

C98 = -std=c++98

all: $(NAME)
$(NAME): $(SRC)
	$(CPP) $(CFLAGS) $(C98) $(SRC) -o $(NAME)
	@clear
	@echo "Compilation complete."

clean:
	@echo "Nothing to clean"
	@echo "Try 'make fclean' instead"

fclean: clean
	rm -f $(NAME)

re: fclean all

run: all
	@./$(NAME)

val: all
	@valgrind --leak-check=full --show-leak-kinds=all ./$(NAME)

.SILENT:

.DEFAULT:
	@echo "Usage: make [ all | clean | fclean | re | run | val ]"

.PHONY: all clean fclean re run val