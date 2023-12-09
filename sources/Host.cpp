/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Host.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cschmied <cschmied@student.42wolfsburg.d>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 11:08:03 by cschmied          #+#    #+#             */
/*   Updated: 2023/09/14 11:10:24 by cschmied         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <webserv.hpp>

Host::Host(std::istream & configuration) : _router(NULL) {
	std::stringstream cpy;

	cpy.clear();
	cpy << configuration.rdbuf();
	configuration.clear();
	configuration.seekg(0, std::ios_base::beg);
	_config = Parser::parseBlock(cpy);

#ifdef DEBUG
	std::cout << SYS_MSG << GREEN"Debug: initializing host with config: \n"R;
	printMap(_config);
	std::cout << "\n";
#endif

	_maxBodySize = Parser::parseInt(_config, "client_max_body_size");
	_name = Parser::parseWord(_config, "server_name");

	cpy.clear();
	cpy << configuration.rdbuf();
	configuration.clear();
	configuration.seekg(0, std::ios_base::beg);
	_router = new Router(cpy);

	Parser::parseHost(*this, _config);
}

std::string Host::createErrorHeader(int errorCode) {
	std::stringstream output;

	output << "HTTP/1.1 " << errorCode;
	switch (errorCode) {
		case 403:
			output << " Forbidden";
			break;
		case 404:
			output << " Not Found";
			break;
		case 429:
			output << " Too Many Requests";
			break;
		case 502:
			output << " Bad Gateway";
			break;
		case 503:
			output << " Service Unavailable";
			break;
		case 504:
			output << " Gateway Timeout";
			break;
		default: // 500
			output << " Internal Server Error";
	}
	output << "\n";
	output << "Server: 42webserv\n";
	output << "Content-Type: text/html; charset=utf-8\n";
	output << "Access-Control-Allow-Origin: *\n";
	output << "Connection: close\n";
	return output.str();
}


void Host::sendErrorPage(int fd, int error) {
	std::stringstream errorPagePath;
	std::string errorHeader;

	errorHeader = createErrorHeader(error);
	errorHeader += "\n\n";
	write(fd, errorHeader.c_str(), errorHeader.size());
	if (_error_pages.find(error) != _error_pages.end()) {
		if (writeFiletoFd(fd, _error_pages[error].c_str()) == 0) {
			return ;
		}
	}
	errorPagePath << PATH_STD_ERRORPAGES  << "_site/" << error << ".html";
	if (writeFiletoFd(fd, errorPagePath.str().c_str()) != 0) {
		//errorHeader = createErrorHeader(500).c_str();
		errorHeader += "\n\nInternal Server Error";
		write(fd, errorHeader.c_str(), errorHeader.size());
	}
}

Host::~Host() {
//	delete _router;
}

const std::string &Host::getIp() const {
	return _IP;
}

void Host::setPort(double port) {
	_port = port;
}

void Host::setIp(const std::string &ip) {
	_IP = ip;
}

double Host::getPort() const {
	return _port;
}

const std::string &Host::getName() const {
	return _name;
}

Router *Host::getRouter() const {
	return _router;
}
