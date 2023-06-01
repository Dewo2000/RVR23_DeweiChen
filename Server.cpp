#include "Server.h"
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>


Server::Server(int port) : port(port), socketFD(-1) {}

Server::~Server() {
    stop();
}

bool Server::start() {
    // Crear socket
    socketFD = socket(AF_INET, SOCK_STREAM, 0);
    if (socketFD == -1) {
        std::cerr << "Failed to create socket" << std::endl;
        return false;
    }

    // Establecer la dirección del servidor
    sockaddr_in serverAddress{};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(port);

    // Vincular el socket a la dirección del servidor
    if (bind(socketFD, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        std::cerr << "Failed to bind socket" << std::endl;
        return false;
    }

    // Escuchar conexiones entrantes
    if (listen(socketFD, 2) < 0) {
        std::cerr << "Failed to listen for connections" << std::endl;
        return false;
    }

    std::cout << "Server started, waiting for connections..." << std::endl;

    // Aceptar la primera conexión entrante
    int clientSocket = accept(socketFD, nullptr, nullptr);
    if (clientSocket < 0) {
        std::cerr << "Failed to accept connection" << std::endl;
        return false;
    }

    std::cout << "Client connected" << std::endl;

    // Game loop
    bool quit = false;
    while (!quit) {
        int paddlePosition;
        int bytesRead = recv(clientSocket, &paddlePosition, sizeof(paddlePosition), 0);
        if (bytesRead <= 0) {
            std::cerr << "Connection closed by client" << std::endl;
            quit = true;
            break;
        }

        // Procesar la posición de la paleta recibida
        // Puedes implementar la lógica del juego aquí

        // Enviar respuesta al cliente
        send(clientSocket, &paddlePosition, sizeof(paddlePosition), 0);
    }

    // Cerrar el socket de cliente
    close(clientSocket);

    return true;
}

void Server::stop() {
    if (socketFD != -1) {
        close(socketFD);
        socketFD = -1;
    }
}
