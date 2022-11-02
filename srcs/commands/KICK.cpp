/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   KICK.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amontaut <amontaut@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/01 11:42:08 by ali               #+#    #+#             */
/*   Updated: 2022/11/02 12:17:55 by amontaut         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/command.hpp"

void	KICK(User* user)
{
	std::vector<std::string> params = user->param_list;

	if (params.size() < 2)
		return user->get_server()->to_send(ERR_NEEDMOREPARAMS(getArgs(1, "KICK"),
					user->get_nick()), user->get_fd());

	Channel* chan = user->get_server()->get_channel(params[0]);
	if (chan == NULL)
		return user->get_server()->to_send(ERR_NOSUCHCHANNEL(getArgs(1, params[0].c_str()),
					user->get_nick()), user->get_fd());

	if (chan->isHere(user->get_nick()) == false)
		return user->get_server()->to_send(ERR_NOTONCHANNEL(getArgs(1, params[0].c_str()),
				user->get_nick()), user->get_fd());

	if (chan->isChanOp(user->get_nick()) == false)
		return user->get_server()->to_send(ERR_CHANOPRIVSNEEDED(getArgs(1, params[0].c_str()),
					user->get_nick()), user->get_fd());

	if (chan->isHere(params[1]) == false)
		return user->get_server()->to_send(ERR_USERNOTINCHANNEL(getArgs(2, params[1].c_str(), params[0].c_str()),
					user->get_nick()), user->get_fd());

	user->get_server()->to_send(getMsg(user, "KICK", params), chan->getFds());
	chan->kickUser(params[1]);
	if (chan->getUserNum() == 0)
		user->get_server()->deleteChannel(chan->getName());
}
