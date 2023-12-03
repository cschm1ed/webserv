/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Logger.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cschmied <cschmied@student.42wolfsburg.d>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 11:30:06 by cschmied          #+#    #+#             */
/*   Updated: 2023/09/15 11:30:33 by cschmied         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <unistd.h>
#include <iostream>
#include <fcntl.h>
#include <colors.hpp>
#include <MyException.hpp>

#define ERROR 1
#define INFO 0
#define REQUEST 2
#define RESPONSE 3

class Logger {

static int logfile;

public:
	Logger();
	~Logger();

	void log(const std::string & entity, const std::string & message, int level);

	class OpenLogfileFailed : public std::exception {
	public:
		virtual const char *what() const throw() ;
	};

private:
	int fd;
};

#endif
