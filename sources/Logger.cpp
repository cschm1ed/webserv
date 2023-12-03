/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Logger.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cschmied <cschmied@student.42wolfsburg.d>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 11:30:06 by cschmied          #+#    #+#             */
/*   Updated: 2023/09/15 11:33:42 by cschmied         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Logger.hpp>

Logger::Logger() {
	fd = open("webserv_log.log", O_CREAT | O_WRONLY, 0644);
	if (fd == -1) {
		throw MyException("open() on logfile failed", __FILE__, __LINE__);
	}
}

Logger::~Logger() {
	close(fd);
}

const char *Logger::OpenLogfileFailed::what() const throw() {
	return "Error: could not open file webserv_log.log\n";
}

void Logger::log(const std::string &entity, const std::string &message, int level) {
	std::string logMessage;
	switch (level) {
		case ERROR:
			logMessage += SYS_MSG;
			logMessage += RED"-- " + entity + "encountered an error --\n" + R;
			break ;
		case INFO:
			logMessage += SYS_MSG;
			logMessage += GREEN"-- " + entity + " --\n" + R;
			break ;
		case REQUEST:
			logMessage += MAGENTA"-- " + entity + " recieved a request --\n" + R;
			break ;
		case RESPONSE:
			logMessage += BLUE"-- " + entity + " sent a response --\n" + R;
			break ;
		default:
			logMessage += "-- log message --\n";
			break ;
	}
	logMessage += message + "\n";
	write(fd, logMessage.c_str(), logMessage.size());
}
