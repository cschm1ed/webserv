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

typedef struct s_request t_request;

class Router;
class Host;
class Parser;
class Server;

#include <iostream>
#include <map>
#include <vector>
#include <map.tpp>
#include <istream>
#include <unistd.h>
#include <MyException.hpp>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <sys/fcntl.h>
#include <vector>
#include <netinet/in.h>
#include <colors.hpp>
#include <sstream>
#include <iosfwd>
#include <cstring>
#include <cctype>
#include <vector.tpp>
#include <map>
#include <fstream>
#include <Router.hpp>
#include <Host.hpp>
#include <Server.hpp>
#include <Router.hpp>
#include <Parser.hpp>

#ifdef __linux__
# include <sys/sendfile.h>
#endif

#define PATH_STD_ERRORPAGES "./resources/error_pages/"
#define MAX_CONNECTIONS 5000
#define INVALID -1
#define DONE 1
#define SEND 0;

typedef struct s_request {
	std::string requestLine;
	std::map<std::string, std::string> header;
	const char *requestBody;
	std::string returnFile;
	int state;
} t_request;

int writeFiletoFd(int fdOut, const char *path);
int handleRequest(int fd);
int handleGetRequest(t_request & request, int fd);

#endif
