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

#include <MyException.hpp>

MyException::MyException(std::string message, const char *file, int line) {
	std::stringstream ss;

	ss << ITALIC << SYS_MSG << RED << "An exception occured. file: '" <<
		R << file << ITALIC << SYS_MSG << RED << "' line: '" << R << line << ITALIC << SYS_MSG << RED << "'\n" << R;
	ss << RED << message << "\n" << R;
	_msg = ss.str();
}

MyException::~MyException() {

}

//MyException::MyException(const MyException &other) {
//	_msg = other._msg;
//}
//
//MyException &MyException::operator=(const MyException &other) {
//	(void)other;
//	return *this;
//}

const char *MyException::what() const throw() {
	return _msg.c_str();
}
