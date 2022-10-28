/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getArgs.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ali <ali@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 15:06:39 by ali               #+#    #+#             */
/*   Updated: 2022/10/28 15:48:02 by ali              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdarg.h>
#include <iostream>
#include <vector>

const std::vector<std::string>	getArgs(int size, ...)
{
	std::vector<std::string> args;

	va_list	ap;
	va_start(ap, size);
	for (int i = 0; i < size; i++)
		args.push_back(va_arg(ap, std::string));
	va_end(ap);

	return args;
}
