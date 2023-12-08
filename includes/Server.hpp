/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cschmied <cschmied@student.42wolfsburg.d>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 16:14:47 by cschmied          #+#    #+#             */
/*   Updated: 2023/09/25 16:14:57 by cschmied         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_SERVER_HPP
#define WEBSERV_SERVER_HPP

#include <webserv.hpp>
#include <RequestHandler.hpp>

typedef struct s_socket {
	sockaddr_in socket;
	int fd;
	int port;
	std::string IP;
} t_socket;

class Server {
public:
	Server(std::string const & config);
    ~Server();

    Server(const Server& other);
    Server& operator=(const Server& other);
	void run();


private:

	void setupSockets(void);
	int setFindMaxFd(fd_set &set);

	std::vector<Host>::const_iterator *socketHostPairs;
	std::map<int, Host*> _clientFdToSocketOwner;
	std::vector<t_socket > _sockets;
    std::vector<Host> _hosts;
	fd_set _incoming, _ready;
	socklen_t _clientSocketSize;
};

#endif //WEBSERV_SERVER_HPP
