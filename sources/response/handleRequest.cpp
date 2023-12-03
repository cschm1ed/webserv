/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handleRequest.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cschmied <cschmied@student.42wolfsburg.d>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 13:57:06 by cschmied          #+#    #+#             */
/*   Updated: 2023/09/13 13:57:06 by cschmied         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <webserv.hpp>
#include <colors.hpp>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include <sstream>
#include <map.tpp>
#include <stdio.h>

static int fdToStringstream(int fd, std::stringstream & request);

int handleRequest(int fd) {
	std::stringstream requestStream;
	t_request request;

	fdToStringstream(fd, requestStream);
	std::getline(requestStream, request.requestLine);

	{
		std::string line;
		while (std::getline(requestStream, line)) {
			if (requestStream.fail()) {
				perror("getline(): ");
				return -1;
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
	std::cout << GREEN << "got request: " << request.requestLine << std::endl;
	printMap(request.header);
	handleGetRequest(request, fd);
	return 0;
}



static int fdToStringstream(int fd, std::stringstream & request) {
	char buffer[1024];
	int readBytes;

	do  {
		readBytes = read(fd, buffer, 1023);
		if (readBytes < 0) {
			perror("read(): ");
			return -1;
		}
		request << buffer;
	} while (readBytes == 1024);
	return 1;
}
