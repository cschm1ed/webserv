/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cschmied <cschmied@student.42wolfsburg.d>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/08 20:19:08 by cschmied          #+#    #+#             */
/*   Updated: 2023/12/09 10:53:11 by cschmied         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <webserv.hpp>


void RequestHandler::handleRequest(int fd, Host &socketOwner) {
	t_request request = parseRequest(fd);
	int state;

	if ((state = checkBodySize(socketOwner, fd)) == 0) {
		socketOwner.sendErrorPage(fd, 400);
		return ;
	}
	else if (state == -1) {
		socketOwner.sendErrorPage(fd, 500);
		return ;
	}
	if (request.splitRequestLine.empty() || request.header["Host"] != socketOwner.getName()) {
		//<editor-fold desc="logging">
		std::cout << RED << "error request on wrong host, sending " << state << std::endl;
		std::cout << "Request for Host: '" << request.header["Host"] << "'";
		std::cout << " | Socket Owner Name: '" << socketOwner.getName() << "'" << std::endl;
		//</editor-fold>
		socketOwner.sendErrorPage(fd, 404);
		return ;
	}
	if ((state = socketOwner.getRouter()->checkRequestLine(request)) != 200) {
		socketOwner.sendErrorPage(fd, state);
		//<editor-fold desc="logging">
		std::cout << RED << "invalid request or forbidden access\n" << R;
		//</editor-fold>
		return ;
	}
	socketOwner.serveRequest(fd, request);
	return ;
}

t_request RequestHandler::parseRequest(int fd) {
	t_request request;
	std::string line;

	std::stringstream requestStream(readHeader(fd));
	std::getline(requestStream, line);
	if (requestStream.fail()) {
		return request;
	}
	request.splitRequestLine = Parser::splitByWhitespace(line);
	{
		while (std::getline(requestStream, line)) {
			line.erase(std::remove(line.begin(), line.end(), '\r'));
			if (requestStream.fail()) {
				perror("getline(): ");
				return request;
			}
			if (requestStream.eof() || line.empty()) {
				break;
			}
			unsigned long pos = line.find(": ", 0);
			request.header[line.substr(0, pos)] = line.substr(pos + 2);
		}
	}
	request.socketFd = fd;
	request.isCGIRequest = false; //default
	//<editor-fold desc="logging">
	std::cout << GREEN << "got request: ";
	printVector(request.splitRequestLine);
	printMap(request.header);
	//</editor-fold>
	return request;
}

std::string RequestHandler::readHeader(int fd) {
	std::string requestString;
	int bytesRead;
	char buffer[2];

	while ((bytesRead = read(fd, buffer, 1)) > 0) {
		requestString += *buffer;
		if (requestString.find("\r\n\r\n") != std::string::npos) {
			return requestString;
		}
	}
	if (bytesRead == -1) {
		return "";
	}
	return requestString;
}

int RequestHandler::checkBodySize(Host &socketOwner, int fd) {
	int bytesAvailable;

	if (ioctl(fd, FIONREAD, &bytesAvailable) >= 0) {
		if (bytesAvailable > socketOwner.getMaxBodySize()) {
			std::cout << __FILE__ << "c: "<< __LINE__ << " Bad request, body larger than max_body_size\n";
			return 0;
		}
		return 1;
	}
	//<editor-fold desc="logging">
	std::cout << RED;
	perror("ioctl");
	std::cout << R;
	//</editor-fold>
	return -1;
}
