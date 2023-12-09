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

int RequestHandler::handleRequest(int fd, Host *socketOwner) {
	t_request request = parseRequest(fd);

	if (request.requestLine.empty() || request.header["Host"] != socketOwner->getName()) {
		socketOwner->sendErrorPage(fd, 400);
	}
	return 0;
}

t_request RequestHandler::parseRequest(int fd) {
	std::stringstream requestStream;
	t_request request;

	Parser::fdToStringstream(fd, requestStream);
	std::getline(requestStream, request.requestLine);

	{
		std::string line;

		while (std::getline(requestStream, line)) {
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

	//std::cout << GREEN << "got request: " << request.requestLine << std::endl;
	//	printMap(request.header);
	//std::vector<std::string> requestLine = Parser::splitByWhitespace(request.requestLine);
	return request;
}

//parses the whole request into a map
// add end condition for end of header
// understand what a valid request is
// check validity of map key, value pairs
