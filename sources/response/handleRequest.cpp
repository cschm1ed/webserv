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
	std::vector<std::string> requestLine = Parser::splitByWhitespace(request.requestLine);
	write(fd, "HTTP/1.1 200 OK"
			  "Content-Type: text/html; charset=UTF-8\n\n"
			  "<!DOCTYPE html>\n"
			  "<html>\n"
			  "<body>\n"
			  "<h1>Hello World, Hello 42!</h1>\n"
			  "<picture>\n"
			  "    <source media=\"(min-width:400px)\" srcset=\"resources/assets/images/sillyCat.jpg\">\n"
			  "    <img src=\"/assets/images/sillyCat.jpg\" alt=\"A silly cat\">\n"
			  "</picture>\n"
			  "</body>\n"
			  "</html>", 245);
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
