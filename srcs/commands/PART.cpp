/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PART.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ali <ali@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/01 11:22:03 by ali               #+#    #+#             */
/*   Updated: 2022/11/01 17:36:22 by ali              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command.hpp"

void	PART(User* user)
{
	std::vector<std::string> params = user->param_list;

	if (params.size() == 0)
		return user->get_server()->to_send(ERR_NEEDMOREPARAMS(getArgs(1, "PART"),
					user->get_nick()), user->get_fd());
	Channel* chan = user->get_server()->get_channel(params[0]);
	if (chan == NULL)
		return user->get_server()->to_send(ERR_NOSUCHCHANNEL(getArgs(1, params[0]),
					user->get_nick()), user->get_fd());
	if (chan->isHere(user->get_nick()) == false)
		return user->get_server()->to_send(ERR_NOTONCHANNEL(getArgs(1, params[0]),
				user->get_nick()), user->get_fd());

	
	user->get_server()->to_send(getMsg(user, "PART", params), chan->getOtherFds(user->get_nick()));
	chan->kickUser(user->get_nick());
	if (chan->getUserNum() == 0)
		user->get_server()->deleteChannel(chan->getName());
}
