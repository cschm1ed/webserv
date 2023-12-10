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

Router::Router(std::istream &serverConf) {
	std::vector<std::string> routesConf;
	std::vector<std::string>::const_iterator it;
	std::map<std::string, std::string> routeConf;

	routesConf = Parser::splitBlocks(serverConf, "location");

	//<editor-fold desc="logging">
	std::cout << SYS_MSG GREEN"Debug: initializing router with routes: \n"R;
	//</editor-fold>

	for (it = routesConf.begin(); it != routesConf.end(); ++it) {
		std::stringstream ss;
		ss << *it;

		routeConf = Parser::parseBlock(ss);
		Parser::checkRoute(routeConf);
		_routes.push_back(routeConf);
		//<editor-fold desc="logging">
		std::cout << "\n";
		printMap(routeConf);
		std::cout << "\n";
		//</editor-fold>
	}
}

Router::~Router() {

}

std::map<std::string, std::string> *Router::routeExists(std::string &route) {
	std::vector<std::map<std::string, std::string> >::iterator it;
	std::string root;

	for (it = _routes.begin(); it != _routes.end(); ++it) {
		root = (*it)["location"];
		if (route == "/") {
			if (route == root)
				return &(*it);
		} else if (route.compare(0, root.size(), root, 0, root.size()) == 0)
			return &(*it);
	}
	std::cout << RED <<  "did not find requested route: '"<< route << "'\n" << R;
	return NULL;
}

int Router::checkRequestLine(t_request request) {
	std::map<std::string, std::string> *route;

	if (request.splitRequestLine.size() != 3) {
		//<editor-fold desc="logging">
		std::cout << RED << "ERROR: invalid request line\n";
		std::cout << "request line: ";
		printVector(request.splitRequestLine);
		std::cout << R;
		//</editor-fold>
		return 400;
	}
	route = routeExists(request.splitRequestLine[1]);
	if (!protocolIsSupported(request.splitRequestLine[2])) {
		//<editor-fold desc="logging">
		std::cout << RED << "Error: Requested protocol is not supported\n" << R;
		//</editor-fold>
		return 505;
	}
	if (!route) {
		//<editor-fold desc="logging">
		std::cout << RED << "Error: route does not exist\n" << R;
		//</editor-fold>
		return 404;
	}
	if (!methodIsAllowed(request.splitRequestLine[0], route)) {
		//<editor-fold desc="logging">
		std::cout << RED << "Error: method is not allowed on this route\n" << R;
		//</editor-fold>
		return 403;
	}
	return 200;
}

bool Router::methodIsAllowed(std::string &method, std::map<std::string, std::string> *route) {
	if (route->find("allow_methods") == route->end()) {
		//<editor-fold desc="logging">
		std::cout << "no allowed routes configured\n";
		//</editor-fold>

		return false;
	}
	if ((*route)["allow_methods"].find(method) != std::string::npos) {
		return true;
	}
	//<editor-fold desc="logging">
	std::cout << "method not allowed\n";
	//</editor-fold>
	return false;
}

bool Router::protocolIsSupported(std::string &protocol) {
	if (protocol == "HTTP/1.1")
		return true;
	return false;
}
