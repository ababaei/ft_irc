/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   KICK.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ali <ali@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/01 11:42:08 by ali               #+#    #+#             */
/*   Updated: 2022/11/01 11:50:21 by ali              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/commands.hpp"

void	KICK(User* user)
{
	std::vector<std::string> params = user->param_list;

	if (params.size() < 2)
		return user->get_server()->to_send(ERR_NEEDMOREPARAMS(getArgs(1, "PART"),
					user->get_nick()), user->get_fd());

}
