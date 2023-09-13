/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cschmied <cschmied@student.42wolfsburg.d>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 13:08:57 by cschmied          #+#    #+#             */
/*   Updated: 2023/09/12 17:13:02 by cschmied         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <sys/select.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <colors.hpp>
#include <sstream>
#include <unistd.h>
#include <cstring>
#include <arpa/inet.h>
#include <deque>

#define MAX_CONNECTIONS 5
#define PORT 8080

static int setFindMax(fd_set &set);

static int setupServerSocket(sockaddr_in &serverSocket, int  &serverFD);

int main() {
	sockaddr_in serverSocket, clientSocketTmp;
	int maxFd, serverFd;
	fd_set incoming, ready;
	socklen_t clientSocketSize;
	std::deque<int> clientFds;
	std::deque <sockaddr_in> clientSockets;

	if (setupServerSocket(serverSocket, serverFd) < 0) {
		std::cout << RED << "ERROR: setting up server failed\n";
		return 1;
	}
	FD_ZERO(&incoming);
	FD_SET(serverFd, &incoming);

	while (1) {
		ready = incoming;
		maxFd = setFindMax(ready);

		select(maxFd + 1, &ready, NULL, NULL, NULL);
		for (int fd = 0; fd <= maxFd; fd++) {
			if (FD_ISSET(fd, &ready)) {
				if (fd == serverFd) {
					clientSocketSize = sizeof(clientSocketTmp);
					int clientDescriptor = accept(serverFd, (struct sockaddr *) &clientSocketTmp, &clientSocketSize);
					if (clientDescriptor < 0) {
						perror("accept(): ");
						return 1;
					}
					clientFds.push_back(clientDescriptor);
					FD_SET(clientDescriptor, &incoming);
				} else {
					write(fd, "Hello from Server\n", 19);
					FD_CLR(fd, &incoming);
					close(fd);
					clientFds.erase(std::find(clientFds.begin(), clientFds.end(), fd));
				}
			}
		}

	}
	close(serverFd);
	return 1;
}

static int setupServerSocket(sockaddr_in &serverSocket, int & serverFd) {
	serverFd = socket(AF_INET, SOCK_STREAM, 0);
	socklen_t serverSize = sizeof(serverSocket);

	if (serverFd < 0) {
		perror("socket(): ");
		return -1;
	}
	serverSocket.sin_family = AF_INET;
	serverSocket.sin_addr.s_addr = htonl(INADDR_ANY);
	serverSocket.sin_port = htons(PORT);
	if (bind(serverFd, (struct sockaddr *) &serverSocket, serverSize) < 0) {
		perror("bind(): ");
		return -1;
	}
	if (listen(serverFd, MAX_CONNECTIONS) < 0) {
		perror("listen(): ");
		return -1;
	}
	return 0;
}

static int setFindMax(fd_set &set) {
	for (int i = FD_SETSIZE; i > 0; --i) {
		if (FD_ISSET(i, &set)) {
			return i;
		}
	}
	return 0;
}
