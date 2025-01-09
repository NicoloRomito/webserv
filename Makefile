NAME = webserver

CONFIG_FILE = config/webserv.conf

SRC_DIR = src/
ERRORS_DIR = src/utils/
PARSING_DIR = src/parsing/configParsing/
DIRECTIVES_DIR = src/DirectivesCpp/
CGI_DIR = src/cgi/

FILES = init/init.cpp utils/errorMsg.cpp classes/logic/Request.cpp classes/logic/Response.cpp requestsHandlers.cpp classes/logic/PostFile.cpp
MAIN = main.cpp
UTILS_FILES = utils.cpp
ERRORS_FILES = Errors.cpp
PARSING_FILES = config.cpp parsing.cpp
DIRECTIVES_FILES = Autoindex.cpp ErrorPage.cpp Index.cpp CgiPass.cpp \
					Location.cpp ClientMaxBodySize.cpp Root.cpp Server.cpp \
					Http.cpp Listen.cpp ServerName.cpp 
CGI_FILES = Cgi.cpp CgiUtils.cpp


SRC = $(addprefix $(SRC_DIR), $(FILES))
UTILS = $(addprefix $(ERRORS_DIR), $(UTILS_FILES))
ERRORS = $(addprefix $(ERRORS_DIR), $(ERRORS_FILES))
PARSING = $(addprefix $(PARSING_DIR), $(PARSING_FILES))
DIRECTIVES = $(addprefix $(DIRECTIVES_DIR), $(DIRECTIVES_FILES))
CGI = $(addprefix $(CGI_DIR), $(CGI_FILES))

INCLUDES = MutantStack.hpp MutantStack.tpp

CPP = g++

CFLAGS = -Wall -Wextra -Werror -g

C98 = -std=c++98

all: $(NAME)

$(NAME): $(MAIN) $(SRC) $(UTILS) $(ERRORS) $(PARSING) $(DIRECTIVES) $(CGI)
	$(CPP) $(CFLAGS) $(C98) $(MAIN) $(SRC) $(UTILS) $(ERRORS) $(PARSING) $(DIRECTIVES) $(CGI) -o $(NAME)
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
	@valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(NAME) $(CONFIG_FILE)

debug: all
	@gdb --args ./webserver config/webserv.conf

.SILENT:

.DEFAULT:
	@echo "Usage: make [ all | clean | fclean | re | run | val ]"

.PHONY: all clean fclean re run val