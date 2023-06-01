#ifndef SERVER_H
#define SERVER_H

class Server {
public:
    Server(int port);
    ~Server();

    bool start();
    void stop();

private:
    int port;
    int socketFD;
};

#endif
