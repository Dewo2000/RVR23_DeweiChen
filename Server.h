#include "Socket.h"
#include <string>
#include <unistd.h>
#include <memory>
#include <vector>

class PongServer
{
public:
    PongServer(const char * s, const char * p): socket(s, p)
    {
        socket.bind();
    };

    void do_messages();

private:

    std::vector<std::unique_ptr<Socket>> players;
    std::unique_ptr<Socket> player1;
    std::unique_ptr<Socket> player2;
    int player=0;
    /**
     * Socket del servidor
     */
    Socket socket;
};