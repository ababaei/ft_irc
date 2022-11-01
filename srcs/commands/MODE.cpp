// /* ************************************************************************** */
// /*                                                                            */
// /*                                                        :::      ::::::::   */
/*   MODE.cpp                                           :+:      :+:    :+:   */
// /*                                                    +:+ +:+         +:+     */
// /*   By: ali <ali@student.42.fr>                    +#+  +:+       +#+        */
// /*                                                +#+#+#+#+#+   +#+           */
// /*   Created: 2022/10/28 15:06:42 by ali               #+#    #+#             */
/*   Updated: 2022/11/01 17:35:12 by ali              ###   ########.fr       */
// /*                                                                            */
// /* ************************************************************************** */

#include "command.hpp"
#include "Server.hpp"

// void	setChanMode(User* user, Channel* channel, const std::vector<std::string>& params)
// {
// 	std::string sign = "+";
// 	bool b = true;
		
// 	for (unsigned int i = 1; i < params[1].length(); i++)
// 	{
// 		if (params[1][i] == 'o' || params[1][i] == 'v' || params[1][i] == 'I' || params[1][i] == 'b')
// 		{
// 			if (params.size() < 3)
// 				return user->get_server()->to_send(ERR_NEEDMOREPARAMS(getArgs(1, "MODE"),
// 							user->get_nick()), user->get_fd());
// 			if (params[1][i] == 'I')
// 			{
// 				if (sign == "+")
// 					channel->inviteUser(params[2]);
// 				else
// 					channel->uninviteUser(params[2]);
// 			}
// 			if (params[1][i] == 'b')
// 			{
// 				if (sign == "+")
// 					channel->banUser(params[2]);
// 				else
// 					channel->unbanUser(params[2]);
// 			}
// 			if (channel->isHere(params[2]) == false)
// 				return user->get_server()->to_send(ERR_USERNOTINCHANNEL(getArgs(2, params[2],
// 								params[0]), user->get_nick()), user->get_fd());
// 			if (params[1][i] == 'o')
// 			{
// 				channel->setUserOp(params[2], b);
// 				if (channel->hasOneOp())
// 					user->get_server()->to_send(RPL_UNIQOPIS(getArgs(2, params[0], params[2]),
// 								user->get_nick()), channel->getFds());
// 			}
// 			else if (params[1][i] == 'v')
// 				channel->setUserVoiced(params[2], b);
// 		}
// 		else if (params[1][i] == 'i')
// 		{
// 			channel->setChanFlag("inviteOnly", b);
// 			user->get_server()->to_send(RPL_CHANNELMODEIS(getArgs(2, params[0], sign + "i"),
// 						user->get_nick()), channel->getFds());
// 		}
// 		else if (params[1][i] == 's')
// 		{
// 			channel->setChanFlag("secret", b);
// 			user->get_server()->to_send(RPL_CHANNELMODEIS(getArgs(2, params[0], sign + "s"),
// 						user->get_nick()), channel->getFds());
// 		}
// 		else if (params[1][i] == 'a')
// 		{
// 			channel->setChanFlag("anonymous", b);
// 			user->get_server()->to_send(RPL_CHANNELMODEIS(getArgs(2, params[0], sign + "a"),
// 						user->get_nick()), channel->getFds());
// 		}
// 		else if (params[1][i] == 'm')
// 		{
// 			channel->setChanFlag("moderated", b);
// 			user->get_server()->to_send(RPL_CHANNELMODEIS(getArgs(2, params[0], sign + "m"),
// 						user->get_nick()), channel->getFds());
// 		}
// 		else if (params[1][i] == 'l')
// 		{
// 			if (sign == "+")
// 			{
// 				if (params.size() < 3)
// 					return user->get_server()->to_send(ERR_NEEDMOREPARAMS(getArgs(1, "MODE"),
// 								user->get_nick()), user->get_fd());
// 				std::stringstream ss;
// 				int limit;
// 				ss << params[2];
// 				ss >> limit;
// 				if (limit != 0)
// 				{
// 					channel->setUserLimit(limit);
// 					user->get_server()->to_send(RPL_CHANNELMODEIS(getArgs(3, params[0], "+l", params[2]),
// 								user->get_nick()), channel->getFds());
// 				}
// 			}
// 			else
// 			{
// 				channel->setUserLimit(500);
// 				user->get_server()->to_send(RPL_CHANNELMODEIS(getArgs(2, params[0], "-l"),
// 							user->get_nick()), channel->getFds());
// 			}
// 		}
// 		else if (params[1][i] == 'k')
// 		{
// 			if (sign == "+")
// 			{
// 				if (params.size() < 3)
// 					return user->get_server()->to_send(ERR_NEEDMOREPARAMS(getArgs(1, "MODE"),
// 								user->get_nick()), user->get_fd());
// 				if (channel->getKey() != "")
// 					return user->get_server()->to_send(ERR_KEYSET(getArgs(1, params[0]),
// 								user->get_nick()), user->get_fd());
// 				channel->setKey(params[2]);
// 				user->get_server()->to_send(RPL_CHANNELMODEIS(getArgs(3, params[0], "+k", params[2]),
// 							user->get_nick()), channel->getFds());
// 			}
// 			else
// 			{
// 				channel->setKey("");
// 				user->get_server()->to_send(RPL_CHANNELMODEIS(getArgs(2, params[0], "-k"),
// 							user->get_nick()), channel->getFds());
// 			}
// 		}
// 		else if (params[1][i] == '+' || params[1][i] == '-')
// 		{
// 			char charsign = params[1][i];
// 			if (charsign == '+')
// 			{
// 				sign = "+";
// 				b = true;
// 			}
// 			else
// 			{
// 				sign = "-";
// 				b = false;
// 			}
// 		}
// 		else
// 			user->get_server()->to_send(ERR_UNKNOWNMODE(getArgs(2, "" + params[1][i], params[0]),
// 					user->get_nick()), user->get_fd());
// 	}
// }

// void	chanMode(User* user, std::vector<std::string>& params)
// {
// 	Channel* channel = user->get_server()->get_channel(params[0]);

// 	if (channel == NULL)
// 		return user->get_server()->to_send(ERR_NOSUCHCHANNEL(getArgs(1, params[0]), user->get_nick()),
// 				user->get_fd());

// 	if (channel->isChanOp(user->get_nick()) == false)
// 		return user->get_server()->to_send(ERR_CHANOPRIVSNEEDED(getArgs(1, params[0]), user->get_nick()),
// 				user->get_fd());

// 	setChanMode(user, channel, params);
// }

// void	userMode(User* user, std::vector<std::string>& params)
// {
// 	std::string sign = "+";
// 	bool b = true;

// 	if (user->get_server()->isHere(params[0]) == false)
// 		return ;
// 	for (unsigned int i = 1; i < params[1].length(); i++)
// 	{
// 		if (params[1][i] == 'a')
// 		{
// 			user->set_mode("away", b);
// 			user->get_server()->to_send(RPL_UMODEIS(getArgs(1, sign + "a"), user->get_nick()),
// 					user->get_fd());
// 		}
// 		else if (params[1][i] == 'i')
// 		{
// 			user->set_mode("invisible", b);
// 			user->get_server()->to_send(RPL_UMODEIS(getArgs(1, sign + "i"), user->get_nick()),
// 					user->get_fd());
// 		}
// 		else if (params[1][i] == 'w')
// 		{
// 			user->set_mode("wallops", b);
// 			user->get_server()->to_send(RPL_UMODEIS(getArgs(1, sign + "w"), user->get_nick()),
// 					user->get_fd());
// 		}
// 		else if (params[1][i] == 'r')
// 		{
// 			user->set_mode("restricted", b);
// 			user->get_server()->to_send(RPL_UMODEIS(getArgs(1, sign + "r"), user->get_nick()),
// 					user->get_fd());
// 		}
// 		else if (params[1][i] == 'o')
// 		{
// 			user->set_mode("operator", b);
// 			user->get_server()->to_send(RPL_UMODEIS(getArgs(1, sign + "o"), user->get_nick()),
// 					user->get_fd());
// 		}
// 		else if (params[1][i] == 'O')
// 		{
// 			user->set_mode("local_op", b);
// 			user->get_server()->to_send(RPL_UMODEIS(getArgs(1, sign + "O"), user->get_nick()),
// 					user->get_fd());
// 		}
// 		else if (params[1][i] == 's')
// 		{
// 			user->set_mode("server_notice", b);
// 			user->get_server()->to_send(RPL_UMODEIS(getArgs(1, sign + "s"), user->get_nick()),
// 					user->get_fd());
// 		}
// 		else if (params[1][i] == '+' || params[1][i] == '-')
// 		{
// 			char charsign = params[1][i];
// 			if (charsign == '+')
// 			{
// 				sign = "+";
// 				b = true;
// 			}
// 			else
// 			{
// 				sign = "-";
// 				b = false;
// 			}
// 		}
// 		else
// 			user->get_server()->to_send(ERR_UMODEUNKNOWNFLAG(getArgs(0),
// 						user->get_nick()), user->get_fd());
// 	}
// }

void    MODE(User *user)
{
	std::vector<std::string> params = user->param_list;
	// if (params.size() < 2)
	// 	return user->get_server()->to_send(ERR_NEEDMOREPARAMS(getArgs(1, "MODE"), user->get_nick()),
	// 			user->get_fd());
	// if (isChanName(params[0]))
	// 	chanMode(user, params);
	// else
	// 	userMode(user, params);
}
