#ifndef CLIENT_H
#define CLIENT_H

#include <string>

class Client {
public:
    Client(const std::string& serverIP, int serverPort);
    ~Client();

    bool connectToServer();
    void disconnectFromServer();
    void sendPaddlePosition(int position);

private:
    std::string serverIP;
    int serverPort;
    int socketFD;
};

#endif
