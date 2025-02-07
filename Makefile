NAME = webserver

CONFIG_FILE = config/webserv.conf

SRC_DIR = src/
CLIENT_DIR = src/client/
ERRORS_DIR = src/utils/
PARSING_CONF_DIR = src/parsing/configParsing/
PARSING_DIR = src/parsing/
DIRECTIVES_DIR = src/DirectivesCpp/
CGI_DIR = src/cgi/
LOGIC_DIR = src/classes/logic/

CLIENT_FILES = ClientHandling.cpp ReadFiles.cpp RequestHandler.cpp postHandler.cpp utilsForClientHandling.cpp
FILES = Request.cpp Response.cpp PostFile.cpp Webserv.cpp Client.cpp Upload.cpp
MAIN = main.cpp
UTILS_FILES = utils.cpp printUtils.cpp
ERRORS_FILES = Errors.cpp postReqErrors.cpp
PARSING_CONF_FILES = config.cpp parsing.cpp
PARSING_FILES = parseUpload.cpp parseBody.cpp
DIRECTIVES_FILES = Autoindex.cpp ErrorPage.cpp Index.cpp CgiPass.cpp \
					Location.cpp ClientMaxBodySize.cpp Root.cpp Server.cpp \
					Http.cpp Listen.cpp ServerName.cpp AllowMethods.cpp \
					Rewrite.cpp

CGI_FILES = Cgi.cpp CgiUtils.cpp


LOGIC = $(addprefix $(LOGIC_DIR), $(FILES))
CLIENT = $(addprefix $(CLIENT_DIR), $(CLIENT_FILES))
UTILS = $(addprefix $(ERRORS_DIR), $(UTILS_FILES))
ERRORS = $(addprefix $(ERRORS_DIR), $(ERRORS_FILES))
PARSING_CONF = $(addprefix $(PARSING_CONF_DIR), $(PARSING_CONF_FILES))
PARSING = $(addprefix $(PARSING_DIR), $(PARSING_FILES))
DIRECTIVES = $(addprefix $(DIRECTIVES_DIR), $(DIRECTIVES_FILES))
CGI = $(addprefix $(CGI_DIR), $(CGI_FILES))

INCLUDES = MutantStack.hpp MutantStack.tpp

CPP = g++

CFLAGS = -Wall -Wextra -Werror -g

C98 = -std=c++98

all: $(NAME)

$(NAME): $(MAIN) $(LOGIC) $(CLIENT) $(UTILS) $(ERRORS) $(PARSING) $(DIRECTIVES) $(CGI)
	$(CPP) $(CFLAGS) $(C98) $(MAIN) $(LOGIC) $(CLIENT) $(UTILS) $(ERRORS) $(PARSING_CONF) $(PARSING) $(DIRECTIVES) $(CGI) -o $(NAME)
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
	@valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --track-fds=yes ./$(NAME) $(CONFIG_FILE)

debug: all
	@gdb --args ./webserver config/webserv.conf

.SILENT:

.DEFAULT:
	@echo "Usage: make [ all | clean | fclean | re | run | val ]"

.PHONY: all clean fclean re run val