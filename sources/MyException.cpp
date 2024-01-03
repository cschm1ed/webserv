/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MyException.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cschmied <cschmied@student.42wolfsburg.d>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/26 08:50:36 by cschmied          #+#    #+#             */
/*   Updated: 2023/09/26 08:52:12 by cschmied         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <webserv.hpp>

MyException::MyException(std::string message, const char *file, int line) {
	std::stringstream ss;

	ss << ITALIC << SYS_MSG << RED << "An exception occured. file: '" <<
		R << file << ITALIC << SYS_MSG << RED << "' line: '" << R << line << ITALIC << SYS_MSG << RED << "'\n" << R;
	ss << RED << message << "\n" << R;
	_msg = ss.str();
}

MyException::~MyException() {

}

const char *MyException::what() const throw() {
	return _msg.c_str();
}
