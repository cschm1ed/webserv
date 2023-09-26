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
#include <deque>
#include <sstream>
#include <Logger.hpp>
#include <Server.hpp>

Logger *logger;

int main(int argc, char **argv) {
	Server *server = nullptr;

	if (argc != 2) {
		std::cout << SYS_MSG RED << "ERROR: invalid input\n"R
				  << "expected: './webserv <*.cnf>\n";
		return 0;
	}

	try {
		logger = new Logger();
	} catch (MyException & e) {
		std::cerr << e.what();
		return 1;
	}

	try {
		server = new Server(argv[1]);
	} catch (MyException & e) {
		std::cerr << "Error: " << e.what() << "\n";
		return 1;
	}

	server->run();

	delete server;
}
