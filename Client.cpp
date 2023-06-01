#include "Client.h"
#include <iostream>
#include <unistd.h>
#include "Paddle.h"

Client::Client(const char * s, const char * p)
    : socket(s,p) {}


void Client::sendPaddlePosition(int position) {

}

void Client::net_thread(){
    while(true){
        std::cout<<"net\n";
        Paddle *em;
        socket.recv(*em);
        paddlePos=em->getX();
        std::cout<<"Get Pos\n";
    }   
}

void Client::input_thread(int x,int y){
        Paddle pad;
        pad.type = Paddle::PLAY;
        pad.setPos(x,y);
        socket.send(pad,socket);
}

void Client::login(){
    Paddle pad;
    pad.type== Paddle::START;
    socket.send(pad,socket);
}