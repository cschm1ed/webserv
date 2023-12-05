/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Host.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cschmied <cschmied@student.42wolfsburg.d>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 11:08:04 by cschmied          #+#    #+#             */
/*   Updated: 2023/09/25 16:57:27 by cschmied         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HOST_HPP
#define WEBSERV_HOST_HPP

#include <iostream>
#include <map>
#include <vector>
#include <Router.hpp>
#include <map.tpp>

class Router;

class Host {
public:
	Host(std::istream &configuration);
	~Host();

	const std::string &getIp() const;

	void setIp(const std::string &ip);

	void setPort(double port);

	double getPort() const;

private:

	std::string _name;
	std::string _IP;
	std::map<int, std::string> _error_pages;
	double _maxBodySize;
	double _port;

	std::map<std::string, std::string> _config;
	Router *_router;
};


#endif
