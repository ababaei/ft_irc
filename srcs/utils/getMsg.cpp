/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getMsg.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amontaut <amontaut@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 18:17:03 by ali               #+#    #+#             */
/*   Updated: 2022/11/03 13:38:41 by ali              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/utils.hpp"

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

const std::string	getMsg(User* user, const std::string& command, const std::string& param)
{
    return ":" + user->get_nick() + "!" + user->get_username() + "@"
        + user->get_hostname() + " " + command + " " + param + "\r\n";
}
