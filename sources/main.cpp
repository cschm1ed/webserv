
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <colors.hpp>

#define MAX_CONNECTIONS 5
#define PORT 443

int main() {
    std::cout << "server startup\n";

    int clientDescriptor, socketDescriptor = socket(AF_INET, SOCK_STREAM, 0);

    if (socketDescriptor == 0) {
        std::cout << RED << "ERROR: creating socket failed\n" << R;
        return 1;
    }

    struct sockaddr_in socketAddress, clientSocket;

    socketAddress.sin_family = AF_INET;
    socketAddress.sin_addr.s_addr = INADDR_ANY;
    socketAddress.sin_port = PORT;
    
    if (bind(socketDescriptor, (struct sockaddr *) &socketAddress, sizeof(socketAddress)) < 0) {
        std::cout << RED << "ERROR: binding socket failed\n " << R;
        return 1;
    }

    if (listen(socketDescriptor, MAX_CONNECTIONS) < 0) {
        std::cout << RED << "ERROR: listening on socket failed\n" << R;
        return 1;
    }

    socklen_t clientLen = sizeof(clientSocket);
    if ((clientDescriptor = accept(socketDescriptor, (struct sockaddr *) &clientSocket, &clientLen)) < 0) {
        std::cout << RED << "ERROR: accepting socket failed\n" << R;
        return 1;
    }

}