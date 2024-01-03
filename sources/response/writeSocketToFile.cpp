/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   writeSocketToFile.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cschmied <cschmied@student.42wolfsburg.d>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/03 17:21:03 by cschmied          #+#    #+#             */
/*   Updated: 2024/01/03 17:21:03 by cschmied         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <webserv.hpp>

static inline int getChunkSize(int fdSocket);

void writeSocketToFile(int fdSocket, int fdFile) {
	int ret;
	char buffer[1024];

	while ((ret = read(fdSocket, buffer, 1023)) == 1023) {
		write(fdFile, buffer, ret);
	}
	if (ret > 0) {
		write(fdFile, buffer, ret);
	}
}

int writeChunkedSocketToFile(int fdSocket, int fdFile) {
	int ret, chunkSize, to_read, sum;
	std::stringstream tmpBuffer;
	char buffer[1024];

	do {
		sum = 0;
		chunkSize = getChunkSize(fdSocket);
		if (chunkSize == -1) {
			//<editor-fold desc="logging">
			std::cout << RED << __FILE__ << " c: " << __LINE__ << "failed to write chunked request to file\n" << R;
			//</editor-fold>
			return -1;
		}
		while (sum < chunkSize) {
			to_read = (chunkSize - sum < 1024) ? chunkSize - sum : 1024;
			ret = read(fdSocket, buffer, to_read);
			if (ret < to_read) {
				//<editor-fold desc="logging">
				std::cout << RED << __FILE__ << " c: " << __LINE__ << "failed to write chunked request to file"
																	<< "incorrect chunk size\n" << R;
				//</editor-fold>
				return -1;
			}
			sum += ret;
			write(fdFile, buffer, ret);
		}
	} while (chunkSize > 0);
	return 1;
}

static inline int getChunkSize(int fdSocket) {
	int ret;
	char buffer;
	std::stringstream stream;

	while (stream.str().find("\r\n") == std::string::npos) {
		ret = read(fdSocket, &buffer, 1);
		if (ret == -1) {
			perror("read");
			return -1;
		}
		stream << buffer;
	}
	stream >> ret;
	return ret;
}
