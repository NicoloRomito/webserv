NAME = webserver

SRC_DIR = src/
PARSING_DIR = src/parsing/configParsing/
DIRECTIVES_DIR = src/DirectivesCpp/

FILES = main.cpp
PARSING_FILES = config.cpp parsing.cpp
DIRECTIVES_FILES = Autoindex.cpp ErrorPage.cpp Index.cpp CgiPass.cpp \
					Location.cpp ClientMaxBodySize.cpp Root.cpp Server.cpp \
					Http.cpp Listen.cpp ServerName.cpp 

SRC = $(addprefix $(SRC_DIR), $(FILES))
PARSING = $(addprefix $(PARSING_DIR), $(PARSING_FILES))
DIRECTIVES = $(addprefix $(DIRECTIVES_DIR), $(DIRECTIVES_FILES))

INCLUDES = MutantStack.hpp MutantStack.tpp

CPP = c++

CFLAGS = -Wall -Wextra -Werror -g

C98 = -std=c++98

all: $(NAME)
$(NAME): $(SRC)
	$(CPP) $(CFLAGS) $(C98) $(SRC) $(PARSING) $(DIRECTIVES) -o $(NAME)
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