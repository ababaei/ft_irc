/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getMsg.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ali <ali@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 18:17:03 by ali               #+#    #+#             */
/*   Updated: 2022/11/01 17:39:35 by ali              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"

const std::string    getMsg(User* user, const std::string& command,
        std::vector<std::string>& params)
{
    std::string msg = ":" + user->get_nick() + "!" + user->get_username() + "@"
        + user->get_hostname() + " " + command + " ";
    for (std::vector<std::string>::iterator it = params.begin(); it != params.end();
            it++)
	{
        msg += *it;
        if (it != params.end() - 1)
            msg += " ";
	}
	msg += "\r\n";

    return msg;
}
