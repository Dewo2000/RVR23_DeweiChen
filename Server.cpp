#include "Server.h"
#include "Paddle.h"

int main(int argc, char **argv)
{
    PongServer es(argv[1], argv[2]);

    es.do_messages();

    return 0;
}

void PongServer::do_messages(){
    while(true){

         Paddle em;
         Socket *clientSock = nullptr;
         socket.recv(em,clientSock);
         std::unique_ptr<Socket> uClientSock(clientSock);
         switch(em.type){
            case Paddle::START:
            if(player==0){
                player1=std::move(uClientSock);
                std::cout<<"Player1 login\n";
                player++;
            }
            else{
                player2=std::move(uClientSock);
                std::cout<<"Player2 login\n";
                player++;
            }
                break;
            case Paddle::PLAY:
                if(player==2){
                    if(*player1.get() == *uClientSock.get()){
                        socket.send(em,*player2.get());
                        std::cout<<"Player2 send\n";
                    }
                    else{
                        socket.send(em,*player1.get());
                        std::cout<<"Player1 send\n";
                    }
                }
                break;
         } 
    }
}