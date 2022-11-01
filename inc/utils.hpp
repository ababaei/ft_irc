/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ali <ali@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 10:44:05 by ali               #+#    #+#             */
/*   Updated: 2022/11/01 11:06:23 by ali              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
# define UTILS_HPP

# include "std.hpp"
# include "User.hpp"

class User;

const std::vector<std::string>	getArgs(int size, ...);
bool	isChanName(const std::string& param);
const std::string    getMsg(User* user, const std::string& command,
        std::vector<std::string>& params);

#endif
