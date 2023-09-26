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

#include <Router.hpp>

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
