/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MODE.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ali <ali@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 15:06:42 by ali               #+#    #+#             */
/*   Updated: 2022/10/28 19:08:59 by ali              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/command.hpp"
#include "../../inc/Server.hpp"

#define CHANNEL 0
#define USER 1

void	setChanMode(User* user, Channel* channel, const std::vector<std::string>& params)
{
	char sign = params[1][0];

	bool b;
	if (sign == '+')
		b = true;
	else
		b = false;
		

	for (int i = 1; i < params[1].length(); i++)
	{
		if (params[1][i] == 'o' || params[1][i] == 'v' || params[1][i] == 'I' || params[1][i] == 'b')
		{
			if (params.size < 3)
				return user->get_server()->to_send(ERR_NEEDMOREPARAMS(getArgs(1, "MODE"),
							user->get_nick()), user->get_fd());
			if (params[1][i] == 'I')
			{
				if (sign == '+')
					channel->inviteUser(param[2]);
				else
					channel->uninviteUser(param[2]);
			}
			if (params[1][i] == 'b')
			{
				if (sign == '+')
					channel->banUser(param[2]);
				else
					channel->unbanUser(param[2]);
			}
			if (channel->isHere(params[2]) == false)
				return user->get_server()->to_send(ERR_USERNOTINCHANNEL(getArgs(2, params[2],
								params[0]), user->get_nick()), user->get_fd());
			if (params[1][i] == 'o')
			{
				channel->setUserOp(param[2], b);
				if (channel->hasOneOp())
					user->get_server()->to_send(RPL_UNIQOPIS(getArgs(2, params[0], params[2]),
								user->get_nick()), user->get_fd());
			}
			else if (param[1][i] == 'v')
				channel->setUserVoiced(param[2], b);
		}
		if (params[1][i] == 'i')
		{
			channel->setChanFlag("inviteOnly", b);
			user->get_server()->to_send(RPL_CHANMODEIS(getArgs(2, param[0], sign + "i"),
						user->get_nick()), user->get_fd());
		}
		if (params[1][i] == 's')
		{
			channel->setChanFlag("secret", b);
			user->get_server()->to_send(RPL_CHANMODEIS(getArgs(2, param[0], sign + "s"),
						user->get_nick()), user->get_fd());
		}
		if (params[1][i] == 'a')
		{
			channel->setChanFlag("anonymous", b);
			user->get_server()->to_send(RPL_CHANMODEIS(getArgs(2, param[0], sign + "a"),
						user->get_nick()), user->get_fd());
		}
		if (params[1][i] == 'm')
		{
			channel->setChanFlag("moderated", b);
			user->get_server()->to_send(RPL_CHANMODEIS(getArgs(2, param[0], sign + "m"),
						user->get_nick()), user->get_fd());
		}
		if (params[1][i] == 'l')
		{
			if (sign == '+')
			{
				if (params.size < 3)
					return user->get_server()->to_send(ERR_NEEDMOREPARAMS(getArgs(1, "MODE"),
								user->get_nick()), user->get_fd());
				std::stringstream ss;
				int limit;
				ss << params[2];
				ss >> limit;
				if (limit != 0)
				{
					channel->setUserLimit(limit);
					user->get_server()->to_send(RPL_CHANMODEIS(getArgs(3, param[0], "+l", param[2]),
								user->get_nick()), user->get_fd());
				}
			}
			else
			{
				channel->setUserLimit(500);
				user->get_server()->to_send(RPL_CHANMODEIS(getArgs(2, param[0], "-l"),
							user->get_nick()), user->get_fd());
			}
		}
		if (params[1][i] == 'k')
		{
			if (sign == '+')
			{
				if (params.size < 3)
					return user->get_server()->to_send(ERR_NEEDMOREPARAMS(getArgs(1, "MODE"),
								user->get_nick()), user->get_fd());
				if (channel->getKey != "")
					return user->get_server()->to_send(ERR_KEYSET(getArgs(1, params[0]),
								user->get_nick()), user->get_fd());
				channel->setKey(param[2]);
				user->get_server()->to_send(RPL_CHANMODEIS(getArgs(3, param[0], "+k", param[2]),
							user->get_nick()), user->get_fd());
			}
			else
			{
				channel->setKey("");
				user->get_server()->to_send(RPL_CHANMODEIS(getArgs(2, param[0], "-k"),
							user->get_nick()), user->get_fd());
			}
		}
	}
}

void	chanMode(User* user, std::vector<std::string>& params)
{
	Channel* channel = user->get_server()->get_channel(params[0]);

	if (channel == NULL)
		return user->get_server()->to_send(ERR_NOSUCHCHANNEL(getArgs(1, params[0]), user->get_nick()),
				user->get_fd());

	if (channel->isChanOp(user->get_nick()) == false)
		return user->get_server()->to_send(ERR_CHANOPRIVSNEEDED(getArgs(1, params[0]), user->get_nick()),
				user->get_fd());

	setChanMode(user, channel, params);
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
	if (params[1][0] != '+' && params[1][0] != '-')
		return user->get_server()->to_send(ERR_UNKNOWNMODE(getArgs(2, params[1], params[0]),
					user->get_nick()), user->get_fd());
	if (isChanName(params[0]))
		chanMode(user, params);
	else
		userMode(user, params);
}
