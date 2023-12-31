/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Host.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cschmied <cschmied@student.42wolfsburg.d>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 11:08:04 by cschmied          #+#    #+#             */
/*   Updated: 2023/09/25 16:57:27 by cschmied         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HOST_HPP
#define WEBSERV_HOST_HPP

#include <webserv.hpp>

class Router;

class Host {
public:
	Host(std::istream &configuration);
	~Host();

	const std::string &getIp() const;

	void setIp(const std::string &ip);
	void setPort(double port);
	double getPort() const;
	Router *getRouter() const;
	const std::string &getName() const;
	void sendErrorPage(int fd, int error);
	void serveRequest(int fd, t_request &request);
	double getMaxBodySize() const;

private:
	std::map<int, std::string> _error_pages;
	std::string createErrorHeader(int errorCode);
	std::string createSuccessHeaderGet(t_request &request);
	std::string createSuccessHeaderDelete(t_request &request);
	std::string createSuccessHeaderPost(t_request &request);
	std::string createDirectoryListing(std::string &directory, t_request &request);
	std::string createSuccessHeaderDirListing(std::string & listing);
	void senddir(int fd, t_request & request);
	static int getFileSize(std::string & path);
	void servePostRequest(int fd, t_request & request);
	void serveGetRequest(int fd, t_request &request);
	void serveDeleteRequest(int fd, t_request &request);
	double _maxBodySize;
	double _port;
	void serveCGIRequest(t_request &request);
	std::map<std::string, std::string> _config;
	Router *_router;
	std::string _name;
	std::string _IP;
    static void createClientErrorHeader(std::stringstream &output, int errorCode);
    static void createServerErrorHeader(std::stringstream &output, int errorCode);
    void parseConfig(std::stringstream &cpy, std::istream &configuration);
    void fillConfigStream(std::stringstream &cpy, std::istream &configuration);
    void sendPermissionDeniedError(int fd);
    void sendResourceNotFoundError(int fd);
    void sendRedirectionResponse(int fd, t_request &request);
    void sendSuccessfullGetRequest(int fd, t_request &request);
    void sendServerFailDeleteResponse(int fd);
    void sendSuccesfullDeleteResponse(int fd, t_request &request);
    void sendServerFailPostResponse(int fd);
    void sendSuccessfullPostResponse(int fd, t_request &request);
};


#endif
