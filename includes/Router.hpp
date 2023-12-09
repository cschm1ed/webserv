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

typedef struct s_route {
	std::string location;
	int error_code;
} t_route;

class Router {
public:
	Router(std::istream & serverConf);
	~Router();

	void handleRequestLine(std::vector<std::string> & requestLine, t_request & request);
	t_route getRoute(std::string & requested);

private:
	std::string _name;
	std::vector<std::map<std::string, std::string> > _routes;
	std::string routeExists(std::string & route);
};


#endif //WEBSERV_ROUTER_HPP
