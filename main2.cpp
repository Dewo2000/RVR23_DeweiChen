
#include <string>
#include <thread>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "Ball.h"
#include "Paddle.h"
#include <sys/socket.h>
#include <netdb.h>
#include <string.h> // para memset
#include <iostream>
#include <time.h>
#include <stdio.h>
#include <arpa/inet.h>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int PADDLE_WIDTH = 100;
const int PADDLE_HEIGHT = 10;
const char* FONT_PATH = "arial.ttf";
const int FONT_SIZE = 24;

SDL_Window* gWindow = nullptr;
SDL_Renderer* gRenderer = nullptr;
TTF_Font* gFont = nullptr;
Paddle* playerPaddle = nullptr;
Paddle* opponentPaddle = nullptr;
Ball* ball = nullptr;
bool init()
{
     if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
        SDL_Log("Error al inicializar SDL: %s", SDL_GetError());
        return false;
    }
    if (TTF_Init() == -1)
    {
        printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
        return false;
    }

    // Crear ventana
    gWindow = SDL_CreateWindow("Pong", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    if (!gWindow) {
        SDL_Log("Error al crear la ventana: %s", SDL_GetError());
        return false;
    }

    // Crear renderizador
    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!gRenderer) {
        SDL_Log("Error al crear el renderizador: %s", SDL_GetError());
        return false;
    }

    gFont = TTF_OpenFont(FONT_PATH, FONT_SIZE);
    if (gFont == nullptr)
    {
        printf("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
        return false;
    }

    return true;
}

void close()
{
    TTF_CloseFont(gFont);
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    TTF_Quit();
    SDL_Quit();
}
SDL_Texture* renderText(const std::string& text, SDL_Color color)
{
    SDL_Surface* surface = TTF_RenderText_Solid(gFont, text.c_str(), color);
    if (surface == nullptr)
    {
        printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
        return nullptr;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(gRenderer, surface);
    if (texture == nullptr)
    {
        printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
        return nullptr;
    }

    SDL_FreeSurface(surface);
    return texture;
}

void renderTexture(SDL_Texture* texture, int x, int y)
{
    SDL_Rect renderQuad = { x, y, 0, 0 };
    SDL_QueryTexture(texture, nullptr, nullptr, &renderQuad.w, &renderQuad.h);
    SDL_RenderCopy(gRenderer, texture, nullptr, &renderQuad);
}
void do_msg(int client_sd){
        while(true){
        char buffer[3*sizeof(int)];
        ssize_t bytes =  recv(client_sd,buffer,3*sizeof(int),0);
        if(bytes<=0)return;
        playerPaddle->from_bin(buffer);
        ball->from_bin(buffer+sizeof(int));
        }
}
void SendData(int sd){
    char buffer[sizeof(int)];
    opponentPaddle->to_bin();
    memcpy(buffer,opponentPaddle->data(),sizeof(int));
    send(sd,buffer,sizeof(int),0);
}
int main(int argc, char* args[])
{
    
    if (!init())
    {
        printf("Failed to initialize!\n");
        return 1;
    }

    bool quit = false;
    SDL_Event e;

    int playerScore = 0;
    int opponentScore = 0;

    ball= new Ball(SCREEN_WIDTH, SCREEN_HEIGHT);
    playerPaddle = new Paddle(SCREEN_WIDTH, SCREEN_HEIGHT, PADDLE_WIDTH, PADDLE_HEIGHT, false ,SCREEN_WIDTH / 2 - PADDLE_WIDTH / 2 , SCREEN_HEIGHT - PADDLE_HEIGHT - 10);
    opponentPaddle = new Paddle(SCREEN_WIDTH, SCREEN_HEIGHT, PADDLE_WIDTH, PADDLE_HEIGHT, true,SCREEN_WIDTH / 2 - PADDLE_WIDTH / 2 , 10);

    const std::string ip = args[1];
    const int port = std::stoi(args[2]);

    // Crear el socket
    int sd = socket(AF_INET, SOCK_STREAM, 0);
    if (sd < 0) {
        std::cerr<<"No se pudo crear el socket\n";
    }

    // Especificar la dirección del servidor
    sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(ip.c_str());
    serv_addr.sin_port = htons(port);

    // Conectar al servidor
    if (connect(sd, (sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cerr<<"No se pudo conectar al servidor\n";
    }

    std::cout << "Conectado al servidor." << std::endl;
    
    std::thread ms(do_msg,sd);

    while (!quit)
    {
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT)
            {
                quit = true;
            }
        }
        ball->update(playerPaddle, opponentPaddle, playerScore, opponentScore);
        opponentPaddle->handleInput();
        playerPaddle->update();
        opponentPaddle->update();

        SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
        SDL_RenderClear(gRenderer);

        ball->render(gRenderer);
        playerPaddle->render(gRenderer);
        opponentPaddle->render(gRenderer);

        SDL_Color textColor = { 0xFF, 0xFF, 0xFF };
        std::string playerScoreText = "Player: " + std::to_string(opponentScore);
        std::string opponentScoreText = "Opponent: " + std::to_string(playerScore);
        SDL_Texture* playerScoreTexture = renderText(playerScoreText, textColor);
        SDL_Texture* opponentScoreTexture = renderText(opponentScoreText, textColor);

        renderTexture(playerScoreTexture, 10, SCREEN_HEIGHT/2);
        renderTexture(opponentScoreTexture, SCREEN_WIDTH -160, SCREEN_HEIGHT/2);

        SDL_DestroyTexture(playerScoreTexture);
        SDL_DestroyTexture(opponentScoreTexture);

        SDL_RenderPresent(gRenderer);

        SendData(sd);

    }
    ms.detach();
    delete ball;
    delete playerPaddle;
    delete opponentPaddle;
    close();
    return 0;
}


