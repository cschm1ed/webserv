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

void RequestHandler::handleRequest(int fd, Host *socketOwner) {
	t_request request = parseRequest(fd);
	int state = 0;

	if (request.requestLine.empty() || request.header["Host"] != socketOwner->getName()) {
		//<editor-fold desc="logging">
		std::cout << RED << "error request on wrong host, sending " << state << std::endl;
		std::cout << "Request for Host: '" << request.header["Host"] << "'";
		std::cout << " | Socket Owner Name: '" << socketOwner->getName() << "'" << std::endl;
		//</editor-fold>
		socketOwner->sendErrorPage(fd, 404);
		return ;
	}
	if ((state = socketOwner->getRouter()->checkRequestLine(request)) != 200) {
		socketOwner->sendErrorPage(fd, state);
		//<editor-fold desc="logging">
		std::cout << RED << "invalid request or forbidden access\n" << R;
		//</editor-fold>
		return ;
	}

	return ;
}

t_request RequestHandler::parseRequest(int fd) {
	std::stringstream requestStream;
	t_request request;

	Parser::fdToStringstream(fd, requestStream);
	std::getline(requestStream, request.requestLine);

	{
		std::string line;

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
		std::string tmp = requestStream.str();
		request.requestBody = tmp.c_str();
	}

	//<editor-fold desc="logging">
	std::cout << GREEN << "got request: " << request.requestLine << std::endl;
	printMap(request.header);
	std::vector<std::string> requestLine = Parser::splitByWhitespace(request.requestLine);
	//</editor-fold>
	return request;
}
