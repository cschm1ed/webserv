/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getRequest.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cschmied <cschmied@student.42wolfsburg.d>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 16:55:55 by cschmied          #+#    #+#             */
/*   Updated: 2023/09/13 16:55:55 by cschmied         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <webserv.hpp>
#include <colors.hpp>
#include <unistd.h>
#include <filesystem>

int handleGetRequest(t_request & request, int fd) {
	unsigned long start = request.requestLine.find(' ');
	unsigned long end = request.requestLine.find(' ', start + 1);

	std::string path = request.requestLine.substr(start + 1, end - start - 1);
	(void)fd;
	return 0;
}
