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

#include <webserv.hpp>

int main(int argc, char **argv) {
	Server *server = NULL;

	if (argc != 2) {
		std::cout << SYS_MSG RED << "ERROR: invalid input\n" << R
				  << "expected: './webserv <*.cnf>\n";
		return 0;
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
