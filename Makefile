NAME		= ft_irc

OBJSDIR		= objs
OBJSFILES = $(SRCSFILES:.cpp=.o)
OBJS = $(addprefix $(OBJSDIR)/, $(OBJSFILES))

INCLUDES	= .

SRCSDIR = srcs
SRCSFILES	= 	main.cpp \
			  	Server.cpp \
			  	User.cpp \
				commands/NICK.cpp \
				commands/USER.cpp \
				commands/PASS.cpp \
				commands/MODE.cpp \
				commands/WHOIS.cpp \
				commands/PING.cpp \
				commands/PART.cpp \
				commands/KICK.cpp \
				commands/JOIN.cpp \
				commands/PRIVMSG.cpp \
				replies/getReply.cpp \
				replies/sendReply.cpp \
				utils/checkParam.cpp \
				utils/getArgs.cpp \
				utils/getMsg.cpp \
				Channel.cpp
SRCS = $(addprefix $(SRCSDIR)/,$(SRCSFILES))

$(OBJSDIR)/%.o:		$(SRCSDIR)/%.cpp
			$(CC) $(CFLAGS) -c $< -o $@

CC			= c++

CFLAGS		= #-Wall -Wextra -Werror -std=c++98

all: $(NAME)


$(NAME):	$(OBJS)
			@echo "Assembling $@"
			$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

client:
	$(CC) $(CFLAGS) -o client srcs/client.cpp

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re test
