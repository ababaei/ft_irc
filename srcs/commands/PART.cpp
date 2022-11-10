/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PART.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amontaut <amontaut@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/01 11:22:03 by ali               #+#    #+#             */
/*   Updated: 2022/11/10 17:03:55 by ali              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command.hpp"

void	partChan(std::vector<std::string>& params, User* user, Channel* chan)
{
	if (chan->isHere(user->get_nick()) == false)
		return user->get_server()->to_send(ERR_NOTONCHANNEL(getArgs(chan->getName()),
				user->get_nick()), user->get_fd());
	
	user->get_server()->to_send(getMsg(user, "PART", params), chan->getFds());
	chan->kickUser(user->get_nick());
	if (chan->getUserNum() == 0)
		user->get_server()->deleteChannel(chan->getName());
}

void	PART(User* user)
{
	std::vector<std::string> params = user->param_list;

	if (params.size() == 0)
		return user->get_server()->to_send(ERR_NEEDMOREPARAMS(getArgs("PART"),
					user->get_nick()), user->get_fd());

	std::vector<std::string> chanNames = splitStr(params[0], ",");
	Channel* chan;
	for (std::vector<std::string>::iterator it = chanNames.begin(); it != chanNames.end(); it++)
	{
		chan = user->get_server()->get_channel(params[0]);
		if (chan == NULL)
			user->get_server()->to_send(ERR_NOSUCHCHANNEL(getArgs(params[0]),
					user->get_nick()), user->get_fd());
		partChan(params, user, chan);
	}
}
