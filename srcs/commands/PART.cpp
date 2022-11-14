/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PART.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amontaut <amontaut@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/01 11:22:03 by ali               #+#    #+#             */
/*   Updated: 2022/11/14 15:05:07 by ali              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command.hpp"

void	partChan(std::string& partMsg, User* user, Channel* chan)
{
	if (chan->isHere(user->get_nick()) == false)
		return user->get_server()->to_send(ERR_NOTONCHANNEL(getArgs(chan->getName()),
				user->get_nick()), user->get_fd());
	
	std::vector<std::string> msgParams;
	if (partMsg != "")
		msgParams = getArgs(chan->getName(), partMsg);
	else
		msgParams = getArgs(chan->getName());
	user->get_server()->to_send(getMsg(user, "PART", msgParams), chan->getFds());
	chan->kickUser(user->get_nick());
	if (chan->getUserNum() == 0)
		user->get_server()->deleteChannel(chan->getName());
}

void	PART(User* user)
{
	std::vector<std::string> params = user->param_list;
	std::string partMsg = "";

	if (params.size() < 1)
		return user->get_server()->to_send(ERR_NEEDMOREPARAMS(getArgs("PART"),
					user->get_nick()), user->get_fd());
	if (params.size() > 1)
	{
		std::vector<std::string> msg(params.begin() + 1, params.end());
		partMsg = getStr(msg);
	}

	std::cout << "HERE" << std::endl;
	std::vector<std::string> chanNames = splitStr(params[0], ",");
	Channel* chan;
	for (std::vector<std::string>::iterator it = chanNames.begin(); it != chanNames.end(); it++)
	{
		chan = user->get_server()->get_channel(*it);
		if (chan == NULL)
			user->get_server()->to_send(ERR_NOSUCHCHANNEL(getArgs(chan->getName()),
					user->get_nick()), user->get_fd());
		else
			partChan(partMsg, user, chan);
	}
}
