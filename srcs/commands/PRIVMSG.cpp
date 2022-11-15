#include "../../inc/command.hpp"
#include "../../inc/Server.hpp"

void	chanMsg(Channel* chan, User* user, std::vector<std::string>& params)
{
	if (chan->canSpeak(user->get_nick()) == false)
		return user->get_server()->toSend(ERR_CANNOTSENDTOCHAN(getArgs(params[0]), user->get_nick()), 
				user->get_fd());

	user->get_server()->toSend(getMsg(user, "PRIVMSG", params), chan->getOtherFds(user->get_nick()));
}

void	userMsg(User* recipient, User* user, std::vector<std::string>& params)
{ 
	if (recipient->get_status() == "away")
	{
		std::cout << "is away" << std::endl;
		user->get_server()->toSend(RPL_AWAY(getArgs(recipient->get_nick(), recipient->getAway()),
					user->get_nick()), user->get_fd());
	}
	else
		user->get_server()->toSend(getMsg(user, "PRIVMSG", params), recipient->get_fd());
}

void	PRIVMSG(User* user)
{
	std::vector<std::string> params = user->param_list;

	if (params.size() < 2)
		return user->get_server()->toSend(ERR_NOTEXTTOSEND(getArgs(), user->get_nick()),
				user->get_fd());
	if (isChanName(params[0]))
	{
		Channel* channel = user->get_server()->getChannel(params[0]);
		if (channel == NULL)
			return user->get_server()->toSend(ERR_NORECIPIENT(getArgs("PRIVMSG"),
						user->get_nick()), user->get_fd());
		else
			chanMsg(channel, user, params);
	}
	else
	{
		User* recipient = user->get_server()->getUser(params[0]);
		if (recipient == NULL)
			return user->get_server()->toSend(ERR_NOSUCHNICK(getArgs(params[0]),
						user->get_nick()), user->get_fd());
		else
			userMsg(recipient, user, params);
	}
}

