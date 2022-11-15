/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checkParam.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ali <ali@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 10:45:52 by ali               #+#    #+#             */
/*   Updated: 2022/10/31 10:46:03 by ali              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/utils.hpp"

bool	isChanName(const std::string& param)
{
	// const char *str = param.c_str();
	if (param[0] == '&' || param[0] == '#' || param[0] == '+' || param[0] == '!')
		return true;
	return false;
}