/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cschmied <cschmied@student.42wolfsburg.d>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 11:47:35 by cschmied          #+#    #+#             */
/*   Updated: 2023/09/13 11:47:35 by cschmied         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP
#define WEBSERV_HPP

#include <iostream>
#include <map>

typedef struct s_request {
	std::string requestLine;
	std::map<std::string, std::string> header;
	const char *requestBody;
} t_request;

int writeFiletoFd(int fdOut, const char *path);
int handleRequest(int fd);
int handleGetRequest(t_request & request, int fd);

#endif
