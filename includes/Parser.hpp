/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cschmied <cschmied@student.42wolfsburg.d>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 16:44:05 by cschmied          #+#    #+#             */
/*   Updated: 2023/09/15 16:44:29 by cschmied         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_PARSER_HPP
#define WEBSERV_PARSER_HPP

#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <map>
#include <algorithm>
#include <array>
#include <cctype>
#include <colors.hpp>
#include <iosfwd>
#include <cstring>
#include <Host.hpp>

class Host;

class Parser {
public:

	static std::vector<std::string> splitBlocks(std::istream & ss, std::string const &identifier);
	static std::map<std::string, std::string> parseBlock(std::istream & ss);
	static bool isComment(std::string line);

class ConfigError : public std::exception {
public:
	const char * what() const _NOEXCEPT;
};

	static int parseInt(std::map<std::string, std::string> & config, std::string key);
	static std::string parseWord(std::map<std::string, std::string> &config, std::string key);
	static void parseHost(Host & server, std::map<std::string, std::string> & config);
	static void checkRoute(std::map<std::string, std::string> & conf);

private:

	static std::vector<std::string> parseStringArray();
	static std::map<int, std::string> parseErrorPages(std::map<std::string, std::string> & config);
	static std::string readBlock(std::istream & ss);
	static bool isWhitespace(const char c);
	static bool notWhitespace(const char c);

	~Parser();
	Parser();
};


#endif //WEBSERV_PARSER_HPP
