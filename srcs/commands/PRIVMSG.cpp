/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PRIVMSG.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ali <ali@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 16:59:45 by ali               #+#    #+#             */
/*   Updated: 2022/10/31 19:05:33 by ali              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/command.hpp"
#include "../../inc/Server.hpp"

void	chanMsg(Channel* chan, User* user, std::vector<std::string>& params)
{
	if (chan->canSpeak(user->get_nick()) == false)
		return user->get_server()->to_send(ERR_CANNOTSENDTOCHAN(getArgs(1, params[0]), user->get_nick()), 
				user->get_fd());

	user->get_server()->to_send(getMsg(user, "PRIVMSG", params), chan->getOtherFds(user->get_nick()));
}

void	userMsg(User* recipient, User* user, std::vector<std::string>& params)
{
	user->get_server()->to_send(getMsg(user, "PRIVMSG", params), recipient->get_fd());
}

void	PRIVMSG(User* user)
{
	std::vector<std::string> params = user->param_list;

	if (params.size() < 2)
		return user->get_server()->to_send(ERR_NOTEXTTOSEND(getArgs(0), user->get_nick()),
				user->get_fd());
	if (isChanName(params[0]))
	{
		Channel* channel = user->get_server()->get_channel(params[0]);
		if (channel == NULL)
			return user->get_server()->to_send(ERR_NORECIPIENT(getArgs(1, "PRIVMSG"),
						user->get_nick()), user->get_fd());
		else
			chanMsg(channel, user, params);
	}
	else
	{
		User* recipient = user->get_server()->get_user(params[0]);
		if (recipient == NULL)
			return user->get_server()->to_send(ERR_NOSUCHNICK(getArgs(1, params[0]),
						user->get_nick()), user->get_fd());
		else
			userMsg(recipient, user, params);
	}
}

