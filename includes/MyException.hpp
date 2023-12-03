/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MyException.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cschmied <cschmied@student.42wolfsburg.d>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/26 08:50:36 by cschmied          #+#    #+#             */
/*   Updated: 2023/09/26 08:52:44 by cschmied         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef WEBSERV_MYEXCEPTION_HPP
#define WEBSERV_MYEXCEPTION_HPP

#include <iostream>
#include <sstream>
#include <colors.hpp>

class MyException {
public:
    MyException(std::string message, const char *file, int line);
    ~MyException();
	const char * what() const throw() ;
private:
	std::string _msg;
};

#endif //WEBSERV_MYEXCEPTION_HPP
