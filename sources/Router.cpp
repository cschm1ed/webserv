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

std::map<std::string, std::string> *Router::routeExists(std::string &route) {
	std::vector<std::map<std::string, std::string> >::iterator it;
	std::string root = getRequestedRoot(route);


	for (it = _routes.begin(); it != _routes.end(); ++it) {
		if ((*it)["location"] != "/" && root == (*it)["location"]) {
			return &(*it);
		}
	}
	for (it = _routes.begin(); it != _routes.end(); ++it) {
		if ((*it)["location"] == "/") {
			return &(*it);
		}
	}
	std::cout << RED << __FILE__ << "c: " << __LINE__ << " did not find requested route: '"<< route << "' with root: '" << root << "' \n" << R;
	return NULL;
}

int Router::checkRequestLine(t_request &request) {

	if (request.splitRequestLine.size() != 3) {
		//<editor-fold desc="logging">
		std::cout << RED << __FILE__ << "c:" << __LINE__ << " ERROR: invalid request line\n";
		std::cout << "request line: ";
		printVector(request.splitRequestLine);
		std::cout << R;
		//</editor-fold>
		return 400;
	}
	request.route = routeExists(request.splitRequestLine[1]);
	if (!protocolIsSupported(request.splitRequestLine[2])) {
		//<editor-fold desc="logging">
		std::cout << RED << __FILE__ << "c:" << __LINE__ << " Error: Requested protocol is not supported\n" << R;
		//</editor-fold>
		return 505;
	}
	if (!request.route) {
		//<editor-fold desc="logging">
		std::cout << RED << __FILE__ << "c:" << __LINE__ <<" Error: route does not exist\n" << R;
		//</editor-fold>
		return 404;
	}
	if (!methodIsAllowed(request.splitRequestLine[0], request.route)) {
		//<editor-fold desc="logging">
		std::cout << RED << "Error: method is not allowed on this route\n" << R;
		//</editor-fold>
		return 403;
	}
	request.requestedRoot = (*request.route) ["location"];
	request.requestedResource = getRequestedRessource(request, *request.route);
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
	std::cout << R << __FILE__ << " c: " << __LINE__ << " method: " << method << " not allowed\n";
	std::cout << "route: ";
	printMap(*route);
	//</editor-fold>
	return false;
}

bool Router::protocolIsSupported(std::string &protocol) {
	if (protocol == "HTTP/1.1")
		return true;
	return false;
}

std::string Router::getRequestedRoot(std::string &requestedLocation) {
	if (requestedLocation.find('/', 1) == std::string::npos) {
		return requestedLocation;
	}
	return requestedLocation.substr(0, requestedLocation.find('/', 1));
}

std::string Router::getRequestedRessource(t_request &request, std::map<std::string, std::string> &route) {
	std::string location = request.splitRequestLine[1];

	if (request.requestedRoot != "/") {
		location.erase(0, request.requestedRoot.size());
	}
	location = route["root"] + location;
	//<editor-fold desc="Description">
	std::cout << BLUE << __FILE__ << " c:" << __LINE__ << " requested file: " << location << "\n"R;
	//</editor-fold>
	return location;
}

std::string Router::getIndex(std::string &dirPath, std::map<std::string, std::string> &route) {
	DIR *dirStream;
	struct dirent *dirEntry;

	if ((dirStream = opendir(dirPath.c_str())) == nullptr) {
		return "";
	}
	if (route.find("index") != route.end()) {
		return dirPath + "/" + route["index"];
	}
	while ((dirEntry = readdir(dirStream)) != nullptr) {
		if (strcmp(dirEntry->d_name, "index.html") == 0
			|| strcmp(dirEntry->d_name, "index.php") == 0
			|| strcmp(dirEntry->d_name, "index.htm") == 0) {
			return dirPath + std::string(dirEntry->d_name);
		}
	}
	return "dirListing";
}

Router::~Router() {

}
