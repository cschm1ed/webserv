/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cschmied <cschmied@student.42wolfsburg.d>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 16:14:47 by cschmied          #+#    #+#             */
/*   Updated: 2023/09/25 16:47:08 by cschmied         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <webserv.hpp>

Server::Server(std::string const & config) {
	std::ifstream infile(config.c_str(), std::ios_base::in);
	std::vector<std::string> hosts;
	std::vector<std::string>::iterator it;
	std::vector<t_socket>::const_iterator its;

	if (!infile.is_open()) {
		throw MyException("could not open file", __FILE__, __LINE__);
	}
	hosts = Parser::splitBlocks(infile, "server");
	for (it = hosts.begin(); it != hosts.end(); ++it) {
		std::stringstream ss;

		ss << *it;
		_hosts.push_back(Host(ss));
	}
	setupSockets();
	FD_ZERO(&_incoming);
	for (its = _sockets.begin(); its != _sockets.end(); ++its) {
		FD_SET(its->fd, &_incoming);
	}
}

void Server::setupSockets() {
	std::vector<Host>::const_iterator it;
	socklen_t socketSize = sizeof(sockaddr_in);
	t_socket tmp;
	int optval = 1;

	for (it = _hosts.begin(); it != _hosts.end(); ++it) {
		memset(&tmp.socket, 0, sizeof(tmp.socket));
		tmp.IP = it->getIp();
		tmp.port = it->getPort();
		tmp.fd = socket(AF_INET, SOCK_STREAM, 0);
		if (tmp.fd < 0) {
			throw MyException("socket() failed", __FILE__, __LINE__);
		}
		tmp.socket.sin_family = AF_INET;
		tmp.socket.sin_addr.s_addr = inet_addr(tmp.IP.c_str());
		tmp.socket.sin_port = htons(tmp.port);
		if (bind(tmp.fd, (struct sockaddr *) &tmp.socket, socketSize) < 0) {
			perror("bind(): ");
			throw MyException("bind() failed", __FILE__, __LINE__);
		}
		if (listen(tmp.fd, MAX_CONNECTIONS) < 0)
			throw MyException("listen() failed", __FILE__, __LINE__);
		setsockopt(tmp.fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
		_sockets.push_back(tmp);
	}
}

void Server::run() {
	std::vector<t_socket>::const_iterator it;
	int maxFd, clientDescriptor;
	sockaddr_in clientSocket;
	socklen_t clientSize;
	bool done = false;

	while (1) {
		done = false;
		_ready = _incoming;
		maxFd = setFindMaxFd(_ready);

		select(maxFd + 1, &_ready, NULL, NULL, NULL);
		for (int fd = 0; fd <= maxFd; fd++) {
			if (FD_ISSET(fd, &_ready)) {
				for (it = _sockets.begin(); it != _sockets.end(); ++it) {
					if (it->fd == fd) {
						clientSize = sizeof(sockaddr_in);
						clientDescriptor = accept(it->fd, (struct sockaddr *) &clientSocket, &clientSize);
						_clientFdToSocketOwner[clientDescriptor] = &_hosts[it - _sockets.begin()];
						if (clientDescriptor < 0) {
							perror("accept(): ");
							throw MyException("accepting client failed", __FILE__, __LINE__);
						}
						FD_SET(clientDescriptor, &_incoming);
						done = true;
						break ;
					}
				}
				if (done == false) {
					RequestHandler::handleRequest(fd, *_clientFdToSocketOwner[fd]);
					FD_CLR(fd, &_incoming);
					close(fd);
				}
			}
		}
	}
}

int Server::setFindMaxFd(fd_set &set) {
	for (int i = FD_SETSIZE; i > 0; --i) {
		if (FD_ISSET(i, &set)) {
			return i;
		}
	}
	return 0;
}

Server::~Server() {

}
