/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ali <ali@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 10:44:05 by ali               #+#    #+#             */
/*   Updated: 2022/10/31 10:45:16 by ali              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
# define UTILS_HPP

#include <stdarg.h>
#include <iostream>
#include <vector>
#include <string>

const std::vector<std::string>	getArgs(int size, ...);
bool	isChanName(const std::string& param);

#endif
