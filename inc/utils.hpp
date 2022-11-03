/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amontaut <amontaut@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 10:44:05 by ali               #+#    #+#             */
/*   Updated: 2022/11/03 13:39:15 by ali              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
#define UTILS_HPP

#include "std.hpp"
#include "User.hpp"

class User;

const std::vector<std::string> getArgs(int size, ...);
bool isChanName(const std::string &param);
const std::string getMsg(User *user, const std::string &command,
                         std::vector<std::string> &params);
const std::string getMsg(User *user, const std::string &command,
                         const std::string& params);

#endif
