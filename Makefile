NAME = ircserv

CC = c++

FLAGS = #-g -Werror -Wall -Wextra -std=c++98

SOURCE = main.cpp \
		Server/Server.cpp \
		Client/Client.cpp \
		MessageProtocol/MessageProtocol.cpp \
		Command/*.cpp \

$(NAME): $(SOURCE)
	$(CC) $(FLAGS) $(SOURCE) -o $(NAME)

all: $(NAME)

clean:
	rm -f $(NAME)
	rm -rf ircserv.dSYM

re: clean all

.PHONY: all clean re