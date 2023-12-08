/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Router.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cschmied <cschmied@student.42wolfsburg.d>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 20:18:53 by cschmied          #+#    #+#             */
/*   Updated: 2023/09/15 20:19:16 by cschmied         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <webserv.hpp>

Router::Router(std::istream & serverConf) {
	std::vector<std::string> routesConf;
	std::vector<std::string>::const_iterator it;
	std::map<std::string, std::string> routeConf;

	routesConf = Parser::splitBlocks(serverConf, "location");

#ifdef DEBUG
	std::cout << SYS_MSG GREEN"Debug: initializing router with routes: \n"R;
#endif

	for (it = routesConf.begin(); it != routesConf.end(); ++it) {
		std::stringstream ss;
		ss << *it;

		routeConf = Parser::parseBlock(ss);
		Parser::checkRoute(routeConf);
		_routes.push_back(routeConf);

#ifdef DEBUG
		std::cout << "\n";
		printMap(routeConf);
		std::cout << "\n";
#endif

	}
}

Router::~Router() {

}

std::string Router::routeExists(std::string &route) {
	std::vector<std::map<std::string, std::string> >::iterator it;
	std::string root;

	for (it = _routes.begin(); it != _routes.end(); ++it) {
		root = (*it)["loaction"];
		if (route.compare(0, root.size(), root, 0, root.size()) == 0) {
			return (*it)["root"];
		}
	}
	return "";
}

void Router::handleRequestLine(std::vector<std::string> &requestLine, t_request &request) {
	std::string location;

	if (requestLine.size() != 3) {
		request.state = INVALID;
		return ;
	}
	location = requestLine[1].substr(1, requestLine[1].find('/', 1));
	location = routeExists(location);
	std::cout << BLUE <<  "location: " <<  location << "\n";
	if (location == "") {
		request.state = INVALID;
		return ;
	}

}
