/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   KICK.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amontaut <amontaut@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/01 11:42:08 by ali               #+#    #+#             */
/*   Updated: 2022/11/15 13:36:06 by ali              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/command.hpp"

void	kickUser(Channel* chan, User* user, std::string& nick, std::string& kickMsg)
{
	if (chan->isHere(user->get_nick()) == false)
		return user->get_server()->to_send(ERR_NOTONCHANNEL(getArgs(chan->getName()),
				user->get_nick()), user->get_fd());

	if (chan->isChanOp(user->get_nick()) == false)
		return user->get_server()->to_send(ERR_CHANOPRIVSNEEDED(getArgs(chan->getName()),
					user->get_nick()), user->get_fd());

	if (chan->isHere(nick) == false)
		return user->get_server()->to_send(ERR_USERNOTINCHANNEL(getArgs(nick, chan->getName()),
					user->get_nick()), user->get_fd());

	std::vector<std::string> msgParams;
	if (kickMsg != "")
		msgParams = getArgs(chan->getName(), nick, kickMsg);
	else
		msgParams = getArgs(chan->getName(), nick, user->get_nick());
	user->get_server()->to_send(getMsg(user, "KICK", msgParams), chan->getFds());
	User* kicked = user->get_server()->get_user(nick);
	chan->kickUser(nick);
	if (chan->getUserNum() == 0)
	{
		user->get_server()->to_send(ERR_CANNOTSENDTOCHAN(getArgs(chan->getName()), kicked->get_nick()),
			kicked->get_fd());
		user->get_server()->deleteChannel(chan->getName());
	}
	else
		user->get_server()->to_send(ERR_CANNOTSENDTOCHAN(getArgs(chan->getName()), kicked->get_nick()),
			kicked->get_fd());
}

void	KICK(User* user)
{
	std::vector<std::string> params = user->param_list;
	std::string	kickMsg = "";

	if (params.size() < 2)
		return user->get_server()->to_send(ERR_NEEDMOREPARAMS(getArgs("KICK"),
					user->get_nick()), user->get_fd());
	if (params.size() > 2)
	{
		std::vector<std::string> msg(params.begin() + 2, params.end());
		kickMsg = getStr(msg);
	}

	std::vector<std::string> chanNames = splitStr(params[0], ",");
	std::vector<std::string> userNicks = splitStr(params[1], ",");

	if (chanNames.size() != 1 && chanNames.size() != userNicks.size())
		return user->get_server()->to_send(ERR_NEEDMOREPARAMS(getArgs("KICK"),
					user->get_nick()), user->get_fd());

	Channel* chan;
	for (unsigned int i = 0; i < chanNames.size(); i++)
	{
		chan = user->get_server()->get_channel(chanNames[i]);
		if (chan == NULL)
		{
			user->get_server()->to_send(ERR_NOSUCHCHANNEL(getArgs(chanNames[i]),
					user->get_nick()), user->get_fd());
			continue ;
		}
		if (chanNames.size() == 1)
		{
			for (std::vector<std::string>::iterator it = userNicks.begin(); it != userNicks.end();
					it++)
				kickUser(chan, user, *it, kickMsg);
		}
		else
			kickUser(chan, user, userNicks[i], kickMsg);
	}
}

