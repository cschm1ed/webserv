/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   writeFileToFd.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cschmied <cschmied@student.42wolfsburg.d>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 11:33:55 by cschmied          #+#    #+#             */
/*   Updated: 2023/09/13 11:33:55 by cschmied         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <webserv.hpp>

#ifdef __APPLE__

int writeFiletoFd(int fdOut, const char *path) {
	int fdIn = open(path, O_RDONLY);
	off_t bytesToSend = 0;

	if (fdIn < 0) {
		perror("open(): ");
		std::cout << RED << "ERROR: opening: " << path << "failed\n"R;
		return -1;
	}
	if (sendfile(fdIn, fdOut, 0, &bytesToSend, NULL, 0) < 0) {
		perror("sendfile(): ");
		return -1;
	}
	close(fdIn);
	return 0;
}

#elif defined __linux__

#include <sys/sendfile.h>

int writeFiletoFd(int fdOut, const char *path) {
    int fdIn = open(path, O_RDONLY);
    off_t offset = 0;
    struct stat stat_buf;

    if (fdIn < 0) {
        perror("open(): ");
        std::cout << RED << "ERROR: opening" << path << " failed\n" << R;
        return -1;
    }
    if (fstat(fdIn, &stat_buf) < 0) {
        perror("fstat()");
        close(fdIn);
        return -1;
    }
    ssize_t sent_bytes = sendfile(fdOut, fdIn, &offset, stat_buf.st_size);
    if (sent_bytes < 0) {
        perror("sendfile()");
        close(fdIn);
        return -1;
    }
    close(fdIn);
    return 0;
}

#endif
