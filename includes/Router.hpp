/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Router.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cschmied <cschmied@student.42wolfsburg.d>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 20:18:53 by cschmied          #+#    #+#             */
/*   Updated: 2023/09/15 20:19:02 by cschmied         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef WEBSERV_ROUTER_HPP
#define WEBSERV_ROUTER_HPP

#include <webserv.hpp>

class Router {
public:
	static std::string getIndex(std::string &dirPath, std::map<std::string, std::string> &route);
	static std::string getNewFileName(std::string & dir);
	int checkRequestLine(t_request &request);
	Router(std::istream & serverConf);

	~Router();
private:
	std::string _name;
	std::vector<std::map<std::string, std::string> > _routes;
	std::map<std::string, std::string> *routeExists(std::string & route);
	std::string getRequestedRoot(std::string &requestedLocation);
	std::string getRequestedRessource(t_request &request, std::map<std::string, std::string> &route);
	bool methodIsAllowed(std::string & method, std::map<std::string, std::string> *route);
	bool protocolIsSupported(std::string & protocol);

};


#endif //WEBSERV_ROUTER_HPP
