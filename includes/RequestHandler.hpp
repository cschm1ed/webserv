/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandler.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cschmied <cschmied@student.42wolfsburg.d>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/05 21:07:54 by cschmied          #+#    #+#             */
/*   Updated: 2023/12/05 21:07:58 by cschmied         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef WEBSERV_REQUESTHANDLER_HPP
#define WEBSERV_REQUESTHANDLER_HPP

#include <Host.hpp>

class RequestHandler {
public:
	static int handleRequest(int fd, Host *socketOwner);


private:
	t_request parseRequest(int fd);

	RequestHandler();
	~RequestHandler();
};


#endif //WEBSERV_REQUESTHANDLER_HPP
