/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getArgs.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amontaut <amontaut@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 15:06:39 by ali               #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2022/11/03 16:37:43 by ali              ###   ########.fr       */
=======
/*   Updated: 2022/11/02 17:07:44 by ali              ###   ########.fr       */
>>>>>>> refs/remotes/origin/arvin
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/utils.hpp"

const std::vector<std::string>	getArgs(const std::string& arg0,
		const std::string& arg1,
		const std::string& arg2,
		const std::string& arg3,
		const std::string& arg4,
		const std::string& arg5,
		const std::string& arg6)
{
	std::vector<std::string> args;

<<<<<<< HEAD
	if (arg0 != "")
		args.push_back(arg0);
	if (arg1 != "")
		args.push_back(arg1);
	if (arg2 != "")
		args.push_back(arg2);
	if (arg3 != "")
		args.push_back(arg3);
	if (arg4 != "")
		args.push_back(arg4);
	if (arg5 != "")
		args.push_back(arg5);
	if (arg6 != "")
		args.push_back(arg6);
=======
	va_list	ap;
	va_start(ap, size);
	for (int i = 0; i < size; i++)
		args.push_back(va_arg(ap, std::string));
	va_end(ap);
>>>>>>> refs/remotes/origin/arvin

	return args;
}
