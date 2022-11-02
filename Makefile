NAME		= ft_irc

OBJSDIR		= objs
OBJSFILES = $(SRCSFILES:.cpp=.o)
OBJS = $(addprefix $(OBJSDIR)/, $(OBJSFILES))

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

INCS = inc

DEPENDS = $(OBJS:.o=.d)


all: $(NAME)

$(OBJSDIR)/%.o:		$(SRCSDIR)/%.cpp
					$(CC) $(CCFLAGS)  -c $< -o $@ -I ./$(INCS)

CC			= c++

CFLAGS	= #-Wall -Wextra -Werror -std=c++98


-include	$(DEPENDS)

$(NAME):	$(OBJS)
			@echo "Assembling $@"
			$(CC) $(CFLAGS) -o $(NAME) $(OBJS) -I ./$(INCS)

client:
	$(CC) $(CFLAGS) -o client srcs/client.cpp

clean:
	rm -rf $(OBJS)
	rm -rf $(DEPENDS)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re test
