NAME = webserver

CONFIG_FILE = config/webserv.conf

SRC_DIR = src/
ERRORS_DIR = src/utils/
PARSING_DIR = src/parsing/configParsing/
DIRECTIVES_DIR = src/DirectivesCpp/

MAIN = main.cpp
UTILS_FILES = utils.cpp
ERRORS_FILES = Errors.cpp
PARSING_FILES = config.cpp parsing.cpp
DIRECTIVES_FILES = Autoindex.cpp ErrorPage.cpp Index.cpp CgiPass.cpp \
					Location.cpp ClientMaxBodySize.cpp Root.cpp Server.cpp \
					Http.cpp Listen.cpp ServerName.cpp 

SRC = $(addprefix $(SRC_DIR), $(MAIN))
UTILS = $(addprefix $(ERRORS_DIR), $(UTILS_FILES))
ERRORS = $(addprefix $(ERRORS_DIR), $(ERRORS_FILES))
PARSING = $(addprefix $(PARSING_DIR), $(PARSING_FILES))
DIRECTIVES = $(addprefix $(DIRECTIVES_DIR), $(DIRECTIVES_FILES))

INCLUDES = MutantStack.hpp MutantStack.tpp

CPP = c++

CFLAGS = -Wall -Wextra -Werror -g

C98 = -std=c++98

all: $(NAME)

$(NAME): $(SRC) $(UTILS) $(ERRORS) $(PARSING) $(DIRECTIVES)
	$(CPP) $(CFLAGS) $(C98) $(SRC) $(UTILS) $(ERRORS) $(PARSING) $(DIRECTIVES) -o $(NAME)
	@clear
	@echo "Compilation complete."

clean:
	@echo "Nothing to clean, ciuschi e' idiota"
	@echo "Try 'make fclean' instead"

fclean: clean
	rm -f $(NAME)

re: fclean all

run: all
	@./$(NAME) $(CONFIG_FILE)

val: all
	@valgrind --leak-check=full --show-leak-kinds=all ./$(NAME) $(CONFIG_FILE)

.SILENT:

.DEFAULT:
	@echo "Usage: make [ all | clean | fclean | re | run | val ]"

.PHONY: all clean fclean re run val