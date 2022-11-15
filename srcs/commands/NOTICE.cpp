#include "../../inc/command.hpp"
#include "../../inc/Server.hpp"

void	chanMsg(Channel* chan, User* user, std::vector<std::string>& params)
{
	if (chan->canSpeak(user->get_nick()) == false)
		return user->get_server()->to_send(ERR_CANNOTSENDTOCHAN(getArgs(params[0]), user->get_nick()), 
				user->get_fd());

	user->get_server()->to_send(getMsg(user, "NOTICE", params), chan->getOtherFds(user->get_nick()));
}

void	userMsg(User* recipient, User* user, std::vector<std::string>& params)
{ 
	user->get_server()->to_send(getMsg(user, "NOTICE", params), recipient->get_fd());
}

void	NOTICE(User* user)
{
	std::vector<std::string> params = user->param_list;

	if (params.size() < 2)
		return user->get_server()->to_send(ERR_NOTEXTTOSEND(getArgs(), user->get_nick()),
				user->get_fd());
	if (isChanName(params[0]))
	{
		Channel* channel = user->get_server()->get_channel(params[0]);
		if (channel == NULL)
			return user->get_server()->to_send(ERR_NORECIPIENT(getArgs("NOTICE"),
						user->get_nick()), user->get_fd());
		else
			chanMsg(channel, user, params);
	}
	else
	{
		User* recipient = user->get_server()->get_user(params[0]);
		if (recipient == NULL)
			return user->get_server()->to_send(ERR_NOSUCHNICK(getArgs(params[0]),
						user->get_nick()), user->get_fd());
		else
			userMsg(recipient, user, params);
	}
}

