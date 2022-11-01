/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checkParam.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ali <ali@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 10:45:52 by ali               #+#    #+#             */
/*   Updated: 2022/11/01 17:39:16 by ali              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"

bool	isChanName(const std::string& param)
{
	const char *str = param.c_str();
	if (str[0] == '&' || str[0] == '#' || str[0] == '+' || str[0] == '!')
		return true;
	return false;
}
