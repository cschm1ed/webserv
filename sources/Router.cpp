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

std::map<std::string, std::string> *Router::routeExists(std::string &route) {
	std::vector<std::map<std::string, std::string> >::iterator it;
	std::string root;

	for (it = _routes.begin(); it != _routes.end(); ++it) {
		root = (*it)["loaction"];
		if (route == "/") {
			if (route == root)
				return &(*it);
		}
		else if (route.compare(0, root.size(), root, 0, root.size()) == 0)
			return &(*it);
	}
	return NULL;
}

int Router::checkRequestLine(t_request request) {
	std::string location, method, protocol;
	std::map<std::string, std::string> *route;

	if (request.requestLine.size() != 3) {
		request.state = INVALID;
		return 400;
	}
	method = request.requestLine[0];
	location = request.requestLine[1];
	protocol = request.requestLine[2];
	route = routeExists(location);
	if (!protocolIsSupported(protocol))
		return 505;
	if (!route)
		return 404;
	if (!methodIsAllowed(method, route))
		return 403;
	return 200;
}

bool	Router::methodIsAllowed(std::string &method, std::map<std::string, std::string> *route) {
	if (route->find("allowed_methods") != route->end()) {
		return false;
	}
	if ((*route)["allowed_methods"].find(method) != std::string::npos) {
		return true;
	}
	return false;
}

bool	Router::protocolIsSupported(std::string &protocol) {
	if (protocol == "HTTP/1.1")
		return true;
	return false;
}
