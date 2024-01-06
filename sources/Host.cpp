/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Host.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cschmied <cschmied@student.42wolfsburg.d>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/10 15:39:04 by cschmied          #+#    #+#             */
/*   Updated: 2023/12/17 17:29:29 by cschmied         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <webserv.hpp>

Host::Host(std::istream &configuration) : _router(nullptr) {
    std::stringstream cpy;

    parseConfig(cpy, configuration);
    //<editor-fold desc="Description">
    std::cout << SYS_MSG << GREEN"Debug: initializing host with config: \n"R;
    printMap(_config);
    std::cout << "\n";
    //</editor-fold>
    _maxBodySize = Parser::parseInt(_config, "client_max_body_size");
    _name = Parser::parseWord(_config, "server_name");
    fillConfigStream(cpy, configuration);
    _router = new Router(cpy);
    Parser::parseHost(*this, _config);
}

void Host::fillConfigStream(std::stringstream &cpy, std::istream &configuration) {
    cpy.clear();
    cpy << configuration.rdbuf();
    configuration.clear();
    configuration.seekg(0, std::ios_base::beg);
}

void Host::parseConfig(std::stringstream &cpy, std::istream &configuration) {
    fillConfigStream(cpy, configuration);
    _config = Parser::parseBlock(cpy);
}

void Host::createClientErrorHeader(std::stringstream &output, int errorCode) {
    switch (errorCode) {
        case 403:
            output << " Forbidden";
            break;
        case 404:
            output << " Not Found";
            break;
        case 429:
            output << " Too Many Requests";
            break;
    }
}

void Host::createServerErrorHeader(std::stringstream &output, int errorCode) {
    switch (errorCode) {
        case 502:
            output << " Bad Gateway";
            break;
        case 503:
            output << " Service Unavailable";
            break;
        case 504:
            output << " Gateway Timeout";
            break;
        default: // 500
            output << " Internal Server Error";
    }
}

std::string Host::createErrorHeader(int errorCode) {
    std::stringstream output;

    output << "HTTP/1.1 " << errorCode;
    createClientErrorHeader(output, errorCode);
    createServerErrorHeader(output, errorCode);
    output << "\n";
    output << "Server: 42webserv\n";
    output << "Content-Type: text/html; charset=utf-8\n";
    output << "Access-Control-Allow-Origin: *\n";
    output << "Connection: close\n";
    return output.str();
}

std::string Host::createSuccessHeaderDirListing(std::string &listing) {
    std::stringstream header;

    header << "HTTP/1.1 200 OK\r\n";
    header << "Server: webserv\r\n";
    header << "content-Type: text/html; charset=UTF-8\r\n";
    header << "Content-Length:" << listing.size() << "\r\n";
    header << "Connection: close\r\n";
    header << "\r\n";
    return header.str();
}

std::string Host::createSuccessHeaderGet(t_request &request) {
    std::stringstream header;

    header << "HTTP/1.1 200 OK\r\n";
    header << "Server: webserv\r\n";
    if (request.header["Accept"].find("text/html") != std::string::npos) {
        header << "content-Type: text/html; charset=UTF-8\r\n";
    } else {
        header << "content-type: image/jpeg\r\n";
    }
    header << "Content-Length: " << getFileSize(request.requestedResource) << "\r\n";
    header << "Connection: close\r\n";
    header << "\r\n\n\n";
    return header.str();
}

void Host::sendErrorPage(int fd, int error) {
    std::stringstream errorPagePath;
    std::string errorHeader;

    errorHeader = createErrorHeader(error);
    write(fd, errorHeader.c_str(), errorHeader.size());
    if (_error_pages.find(error) != _error_pages.end()) {
        if (writeFiletoFd(fd, _error_pages[error].c_str()) == 0) {
            return;
        }
    }
    errorPagePath << PATH_STD_ERRORPAGES << "_site/" << error << ".html";
    if (writeFiletoFd(fd, errorPagePath.str().c_str()) != 0) {
        errorHeader = createErrorHeader(500);
        write(fd, errorHeader.c_str(), errorHeader.size());
    }
}

void Host::serveRequest(int fd, t_request &request) {

    if (request.splitRequestLine[0] == "GET") {
        serveGetRequest(fd, request);
        return;
    } else if (request.splitRequestLine[0] == "DELETE") {
        serveDeleteRequest(fd, request);
        return;
    } else if (request.splitRequestLine[0] == "POST") {
        servePostRequest(fd, request);
        return;
    } else {
        sendErrorPage(fd, 400);
    }
}

int Host::getFileSize(std::string &path) {
    std::ifstream in(path.c_str(), std::ifstream::ate | std::ifstream::binary);
    if (!in.is_open())
        return 0;
    return static_cast<int>(in.tellg());
}

void Host::senddir(int fd, t_request &request) {
    std::string index, header, dirListing;

    index = Router::getIndex(request.requestedResource, *request.route);
    if (index != "dirListing") {
        request.requestedResource = index;
        header = createSuccessHeaderGet(request);
        write(fd, header.c_str(), header.size());
        writeFiletoFd(fd, index.c_str());
        return;
    } else if (index == "dirListing" && (*request.route)["autoindex"] == "on") {
        dirListing = createDirectoryListing(request.requestedResource, request);
        header = createSuccessHeaderDirListing(dirListing);
        write(fd, header.c_str(), header.size());
        write(fd, dirListing.c_str(), dirListing.size());
        return;
    }
    sendErrorPage(fd, 404);
}

void Host::sendPermissionDeniedError(int fd) {
    //<editor-fold desc="logging">
    std::cout << __FILE__ << " c:" << __LINE__ << " requested resource permission denied\n";
    //</editor-fold>
    sendErrorPage(fd, 403);
}

void Host::sendResourceNotFoundError(int fd) {
    //<editor-fold desc="logging">
    std::cout << __FILE__ << " c:" << __LINE__ << " requested resource not found\n";
    //</editor-fold>
    sendErrorPage(fd, 404);
}
void Host::sendRedirectionResponse(int fd, t_request &request) {
    std::stringstream resp;
    resp << "HTTP/1.1 307 Temporary Redirect" << "\r\n";
    resp << "Location: " << (*request.route)["redirect"] << "\r\n\r\n";
    write(fd, resp.str().c_str(), resp.str().size());
}

void Host::sendSuccessfullGetRequest(int fd, t_request &request) {
    t_response response;
    response.header = createSuccessHeaderGet(request);
    write(fd, response.header.c_str(), response.header.size());
    writeFiletoFd(fd, request.requestedResource.c_str());
}

void Host::serveGetRequest(int fd, t_request &request) {
    DIR *dirStr;

    if (request.route->find("redirect") != request.route->end()) {
        sendRedirectionResponse(fd, request);
        return;
    }
    dirStr = opendir((request.requestedResource).c_str());
    if (dirStr != nullptr) {
        closedir(dirStr);
        senddir(fd, request);
        return;
    }
    if (access(request.requestedResource.c_str(), F_OK) == -1) {
        sendResourceNotFoundError(fd);
        return;
    } else if (access(request.requestedResource.c_str(), R_OK) == -1) {
        sendPermissionDeniedError(fd);
        return;
    }
    sendSuccessfullGetRequest(fd, request);
}

void Host::sendServerFailDeleteResponse(int fd) {
    //<editor-fold desc="logging">
    std::cout << __FILE__ << " c:" << __LINE__ << " failed deleting resource\n";
    //</editor-fold>
    sendErrorPage(fd, 500);
}

void Host::sendSuccesfullDeleteResponse(int fd, t_request &request) {
    t_response response;

    response.header = createSuccessHeaderDelete(request);
    write(fd, response.header.c_str(), response.header.size());
    //<editor-fold desc="logging">
    std::cout << "serverd delete request: ";
    std::cout << response.header;
    //</editor-fold>
}

void Host::serveDeleteRequest(int fd, t_request &request) {
    t_response response;

    if (access(request.requestedResource.c_str(), F_OK) == -1) {
        sendResourceNotFoundError(fd);
        return;
    } else if (access(request.requestedResource.c_str(), W_OK) == -1) {
        sendPermissionDeniedError(fd);
        return;
    }
    if (std::remove(request.requestedResource.c_str()) != 0) {
        sendServerFailDeleteResponse(fd);
        return;
    }
    sendSuccesfullDeleteResponse(fd, request);
}

void Host::sendServerFailPostResponse(int fd) {
    //<editor-fold desc="Description">
    std::cout << __FILE__ << " c: " << __LINE__ << ": could not serve post request\n";
    std::cout << "open: " << -1 << " write: " << 0 << "\n";
    perror("open/write");
    //</editor-fold>
    sendErrorPage(fd, 500);
}

void Host::servePostRequest(int fd, t_request &request) {
    std::string filename;
    int fdOut = 0;

    filename = Router::getNewFileName((*request.route)["client_body_temp_path"]);
    request.requestedResource = (*request.route)["client_body_temp_path"] + "/" + filename;
    fdOut = open(request.requestedResource.c_str(), O_CREAT | O_WRONLY, 0644);
    if (fdOut == -1) {
        sendServerFailPostResponse(fd);
        return;
    }
    if (request.header["Transfer-Encoding"] == "chunked") {
        if (writeChunkedSocketToFile(fd, fdOut) < 0) {
            close(fdOut);
            sendErrorPage(fd, 400);
            return;
        }
    } else {
        writeSocketToFile(fd, fdOut);
    }
    close(fdOut);
    sendSuccessfullPostResponse(fd, request);
}

void Host::sendSuccessfullPostResponse(int fd, t_request &request) {
    t_response response;
    response.header = createSuccessHeaderPost(request);
    write(fd, response.header.c_str(), response.header.size());
    std::cout << "Succesfully served post request\n";
}

std::string Host::createSuccessHeaderPost(t_request &request) {
    std::stringstream header;

    header << "HTTP/1.1 201 OK\r\n";
    header << "Server: webserv\r\n";
    header << "Connection: close\r\n";
    header << "Location: " << request.requestedResource << "\r\n";
    header << "\r\n";
    return header.str();
}

std::string Host::createSuccessHeaderDelete(t_request &request) {
    std::stringstream header;

    header << "HTTP/1.1 204 OK\r\n";
    header << "Server: webserv\r\n";
    header << "Connection: close\r\n";
    header << "\r\n";
    (void) request;
    return header.str();
}

std::string Host::createDirectoryListing(std::string &directory, t_request &request) {
    std::stringstream dirListing, fileInfo;
    DIR *dirStream;
    struct dirent *dirEntry;

    if ((dirStream = opendir(directory.c_str())) == nullptr || (dirEntry = readdir(dirStream)) == nullptr) {
        return "";
    }
    dirListing << "<!DOCTYPE html>\n"
                  "<html>\n"
                  "<head>\n"
                  "    <title>Index of /directory</title>\n"
                  "</head>\n"
                  "<body>\n"
                  "    <h1>Index of /directory</h1>\n"
                  "    <ul>\n";

    while ((dirEntry = readdir(dirStream)) != nullptr) {
        dirListing << "<li><a href=\"" << (request.splitRequestLine[1] + "/" + dirEntry->d_name) << "\">"
                   << dirEntry->d_name << "</a></li>\n";
    }
    dirListing << "</ul>\n"
                  "</body>\n"
                  "</html>\n";
    closedir(dirStream);
    return dirListing.str();
}

void Host::serveCGIRequest(t_request &request) {
    char **envVars;


    execve(request.requestedResource.c_str(), nullptr, envVars);
}

Host::~Host() {
//delete _router;
}

const std::string &Host::getIp() const {
    return _IP;
}

void Host::setPort(double port) {
    _port = port;
}

void Host::setIp(const std::string &ip) {
    _IP = ip;
}

double Host::getPort() const {
    return _port;
}

const std::string &Host::getName() const {
    return _name;
}

Router *Host::getRouter() const {
    return _router;
}

double Host::getMaxBodySize() const {
    return _maxBodySize;
}
