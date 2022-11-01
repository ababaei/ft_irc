/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getArgs.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amontaut <amontaut@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 15:06:39 by ali               #+#    #+#             */
/*   Updated: 2022/11/01 17:39:25 by ali              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"

const std::vector<std::string>	getArgs(int size, ...)
{
	std::vector<std::string> args;

	va_list	ap;
	va_start(ap, size);
	// for (int i = 0; i < size; i++)
	// 	args.push_back(va_arg(ap, std::string));
	va_end(ap);

	return args;
}
