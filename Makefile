NAME		= ft_irc
SRCSDIR		= .
OBJSDIR		= objs
INCLUDES	= .
SRCS		= srcs/main.cpp \
				srcs/Server.cpp \
				srcs/User.cpp \
				srcs/NICK.cpp \
				srcs/USER.cpp

CC			= c++

CFLAGS		= #-Wall -Wextra -Werror -std=c++98

all: $(NAME)

$(NAME): $(SRCSDIR)/$(SRCS)
	@echo "Assembling $@"
	@$(CC) $(CFLAGS) -o $@ $^

client:
	$(CC) $(CFLAGS) -o client srcs/client.cpp

clean:

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re test
