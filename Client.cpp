#include "Client.h"
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

Client::Client(const std::string& serverIP, int serverPort)
    : serverIP(serverIP), serverPort(serverPort), socketFD(-1) {}

Client::~Client() {
    disconnectFromServer();
}

bool Client::connectToServer() {
    // Crear socket
    socketFD = socket(AF_INET, SOCK_STREAM, 0);
    if (socketFD == -1) {
        std::cerr << "Failed to create socket" << std::endl;
        return false;
    }

    // Establecer la dirección del servidor
    sockaddr_in serverAddress{};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(serverPort);
    if (inet_pton(AF_INET, serverIP.c_str(), &(serverAddress.sin_addr)) <= 0) {
        std::cerr << "Invalid server address" << std::endl;
        return false;
    }

    // Conectar al servidor
    if (connect(socketFD, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        std::cerr << "Failed to connect to server" << std::endl;
        return false;
    }

    return true;
}

void Client::disconnectFromServer() {
    if (socketFD != -1) {
        close(socketFD);
        socketFD = -1;
    }
}

void Client::sendPaddlePosition(int position) {
    if (socketFD != -1) {
        // Enviar la posición de la paleta al servidor
        send(socketFD, &position, sizeof(position), 0);
    }
}
