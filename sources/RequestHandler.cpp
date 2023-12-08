//
// Created by Christian Schmiedt on 05.12.23.
//

#include "../includes/RequestHandler.hpp"
#include "../includes/Host.hpp"

int RequestHandler::handleRequest(int fd, Host *socketOwner) {
	t_request request = parseRequest(fd);
	return 0;
}
