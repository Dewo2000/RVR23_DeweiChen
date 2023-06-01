#ifndef CLIENT_H
#define CLIENT_H

#include <string>
#include "Socket.h"


class Client {
public:
    Client(const char * s, const char * p);
    ~Client(){};

    void sendPaddlePosition(int position);
    int getPaddlePos(){return paddlePos;}
    void net_thread();
    void input_thread(int x, int y);
    void login();
private:
    Socket socket;
    int paddlePos;
};

#endif
