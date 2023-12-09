/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cschmied <cschmied@student.42wolfsburg.d>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 16:44:05 by cschmied          #+#    #+#             */
/*   Updated: 2023/09/18 15:54:13 by cschmied         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <webserv.hpp>

std::vector<std::string> Parser::splitBlocks(std::istream &ss, std::string const &identifier) {
	std::vector<std::string> output;
	std::string line;

	while (std::getline(ss, line)) {
		if (!Parser::isComment(line) && line.find(identifier, 0) != std::string::npos) {
			output.push_back(readBlock(ss));
			line.erase(std::remove(line.begin(), line.end(), '{'), line.end());
			output.back() += line;
		}
	}
	return output;
}

std::string Parser::readBlock(std::istream &ss) {
	std::string line, block;
	int braceCount = 0;

	braceCount++;
	while (braceCount > 0 && std::getline(ss, line)) {
		if (line.find('{', 0) != std::string::npos) {
			braceCount++;
		} else if (line.find('}', 0) != std::string::npos) {
			braceCount--;
		}
		if (braceCount <= 0) {
			break;
		}
		if (!line.empty()) {
			block += line + "\n";
		}
	}
	return block;
}

std::map<std::string, std::string> Parser::parseBlock(std::istream &ss) {
	std::string line, key, value;
	std::map<std::string, std::string> output;
	std::string::iterator firstNonWhitespace, it;

	while (std::getline(ss, line)) {
		if (line.find('{') != std::string::npos) {
			while (std::getline(ss, line) && line.find('}') == std::string::npos) { ;;
			}
		} else if (line.find('#') == std::string::npos && !line.empty()) {
			firstNonWhitespace = std::find_if(line.begin(), line.end(), notWhitespace);
			it = std::find_if(firstNonWhitespace, line.end(), isWhitespace);
			key = line.substr(firstNonWhitespace - line.begin(), it - firstNonWhitespace);
			it = std::find_if(it, line.end(), notWhitespace);
			value = line.substr(it - line.begin());
			output[key] = value;
		}
	}
	return output;
}

void Parser::parseHost(Host &server, std::map<std::string, std::string> &config) {
	std::string port, listen = config["listen"];
	size_t colonPos;
	char *pEnd;
	double portNum;

	if (listen.empty() || (colonPos = listen.find(':')) == std::string::npos) {
		throw MyException("Config invalid. Invalid host", __FILE__, __LINE__);
	} else {
		port = listen.substr(colonPos + 1, listen.size() - colonPos + 1);
		portNum = strtod(port.c_str(), &pEnd);
		if (*pEnd) {
			throw MyException("Config invalid. Invalid host", __FILE__, __LINE__);
		} else {
			server.setIp(listen.substr(0, colonPos));
			server.setPort(static_cast<int>(portNum));
		}
	}
}

int Parser::parseInt(std::map<std::string, std::string> &config, std::string key) {
	double maxBodySizeNum;
	char *pEnd;
	std::string value = config[key];

	if (value.empty()) {
		throw MyException("Invalid integer, key: " + key + " value: " + config[key], __FILE__, __LINE__);
	}
	maxBodySizeNum = strtod(value.c_str(), &pEnd);
	if (*pEnd || maxBodySizeNum < 0) {
		throw MyException("Invalid integer, key: '" + key + "' value: " + config[key], __FILE__, __LINE__);
	}
	return static_cast<int>(maxBodySizeNum);
}

std::string Parser::parseWord(std::map<std::string, std::string> &config, std::string key) {
	std::string value = config[key];
	std::stringstream ss;

	if (value.empty()) {
		throw MyException("Config invalid. argument count invalid. key: '" + key + "' value: '" + value + "'", __FILE__,
						  __LINE__);
	}
	ss << value;
	value.erase(std::remove(value.begin(), value.end(), ' '), value.end());
	value.erase(std::remove(value.begin(), value.end(), '\t'), value.end());
	if (ss.str() != value) {
		throw MyException("Config invalid. argument count invalid. key: '" + key + "' value: '" + value + "'", __FILE__,
						  __LINE__);
	}
	return value;
}

std::map<int, std::string> Parser::parseErrorPages(std::map<std::string, std::string> &config) {
	std::map<int, std::string> output;
	std::map<std::string, std::string>::const_iterator it;
	int code;

	for (it = config.begin(); it != config.end(); ++it) {
		if (it->first.find("error_page_")) {
			if (scanf(it->first.c_str(), "error_page_%d", &code) != strlen("error_page_XXX")
				|| code < 0) {
				throw MyException("Config invalid. error_page invalid", __FILE__, __LINE__);
			}
			output[code] = it->second;
		}
	}
	return output;
}

void Parser::checkRoute(std::map<std::string, std::string> &conf) {
    const char* array[] = {"allow_methods", "root", "autoindex", "index", "cgi_pass", "upload_path", "client_body_temp_path", "rewrite", "location"};
	std::map<std::string, std::string>::const_iterator it;
    std::vector<std::string> allowed(array, array + sizeof(array) / sizeof(array[0]));

	for (it = conf.begin(); it != conf.end(); ++it) {
		if (std::find(allowed.begin(), allowed.end(), (it->first)) == allowed.end()) {
			throw MyException("Config invalid. identifier: " + it->first + " invalid", __FILE__, __LINE__);
		}
	}
}

bool Parser::isComment(std::string line) {
	std::string::const_iterator it;

	for (it = line.begin(); it != line.end(); ++it) {
		if (isWhitespace(*it)) continue;
		if (*it == '#') return true;
		else
			break;
	}
	return false;
}

std::vector<std::string> Parser::splitByWhitespace(const std::string& input) {
	std::vector<std::string> result;

	std::string::const_iterator start = input.begin();
	std::string::const_iterator end = input.begin();

	while (end != input.end()) {
		if (isWhitespace(*end)) {
			if (start != end) {
				result.push_back(std::string(start, end));
			}
			start = end + 1;
		}
		++end;
	}

	if (start != end) {
		result.push_back(std::string(start, end));
	}

	return result;
}

int Parser::fdToStringstream(int fd, std::stringstream & request) {
	char buffer[1024];
	int readBytes;


	do  {
		readBytes = read(fd, buffer, 1023);
		if (readBytes < 0) {
			perror("read(): ");
			return -1;
		}
		request << buffer;
	} while (readBytes == 1024);
	return 1;
}

const char *Parser::ConfigError::what() const throw() {
	return "ERROR: invalid configuration file\n";
}

bool Parser::isWhitespace(const char c) {
	return std::isspace(c);
}

bool Parser::notWhitespace(const char c) {
	return !std::isspace(c);
}
