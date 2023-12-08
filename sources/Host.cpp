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
