NAME = ircserv

CC = c++

FLAGS = -Werror -Wall -Wextra

SOURCES = $(wildcard *.cpp */*.cpp)
OBJECTS = $(SOURCES:.cpp=.o)

$(NAME): $(OBJECTS)
	$(CC) $(FLAGS) $(OBJECTS) -o $(NAME)

%.o: %.cpp
	$(CC) $(FLAGS) -c $< -o $@

all: $(NAME)

clean:
	rm -f $(OBJECTS)

fclean: clean
	rm -f $(NAME)
	rm -rf ircserv.dSYM

re: fclean all

.PHONY: all clean fclean re
