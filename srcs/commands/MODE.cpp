/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MODE.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ali <ali@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 15:06:42 by ali               #+#    #+#             */
/*   Updated: 2022/10/28 16:44:29 by ali              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/command.hpp"
#include "../../inc/Server.hpp"

#define CHANNEL 0
#define USER 1

bool	setMode(User* user, unsigned short type, const std::vector<std::string>& params)
{
	char sign = params[1][0];

	if (sign != '+' && sign != '-')
		return false;
	if (type == CHANNEL)
	{
		for (int i = 1; i < params[1].length(); i++)
		{
			if (params[1][i] == 'o')
				setChanOp(user, params)
		}

	}
}

void	chanMode(User* user, std::vector<std::string& params)
{
	Channel* channel = user->get_server().get_channel(param[0]);
	if (channel == NULL)
		return user->get_server()->to_send(ERR_NOSUCHCHANNEL(getArgs(1, params[0]), user->get_nick());
	if (setMode(user, CHANNEL, params) == false)
		return user->get_server()->to_send(ERR_UNKNOWNMODE(getArgs(2, params[1],
						params[0]), user->get_nick()));

}

void	userMode(User* user, std::vector<std::string>& params)
{

}

void    MODE(User *user)
{
	std::vector<std::string> params = user->param_list;
	if (params.size() < 2)
		return user->get_server()->to_send(ERR_NEEDMOREPARAMS(getArgs(1, "MODE"), user->get_nick()),
				user->get_fd());
	if (isChanName(params[0]))
		chanMode(user, params);
	else
		userMode(user, params);
}
