#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "Ball.h"
#include "Paddle.h"
#include <string>
#include "Client.h"
#include <thread>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int PADDLE_WIDTH = 100;
const int PADDLE_HEIGHT = 10;
const char* FONT_PATH = "arial.ttf";
const int FONT_SIZE = 24;

SDL_Window* gWindow = nullptr;
SDL_Renderer* gRenderer = nullptr;
TTF_Font* gFont = nullptr;

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

    Ball* ball = new Ball(SCREEN_WIDTH, SCREEN_HEIGHT);
    Paddle* playerPaddle = new Paddle(SCREEN_WIDTH, SCREEN_HEIGHT, PADDLE_WIDTH, PADDLE_HEIGHT, true);
    Paddle* opponentPaddle= new Paddle(SCREEN_WIDTH, SCREEN_HEIGHT, PADDLE_WIDTH, PADDLE_HEIGHT, false);

    Client cl(args[1],args[2]);

    cl.login();
    
    std::thread net_thread([&cl](){ cl.net_thread(); });

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
        playerPaddle->handleInput();
        playerPaddle->update();
        opponentPaddle->update();
        //cl.net_thread();
        opponentPaddle->setPos(cl.getPaddlePos(),opponentPaddle->getY());
        cl.input_thread(playerPaddle->getX(),playerPaddle->getY());

        SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
        SDL_RenderClear(gRenderer);

        ball->render(gRenderer);
        playerPaddle->render(gRenderer);
        opponentPaddle->render(gRenderer);

        SDL_Color textColor = { 0xFF, 0xFF, 0xFF };
        std::string playerScoreText = "Player: " + std::to_string(playerScore);
        std::string opponentScoreText = "Opponent: " + std::to_string(opponentScore);
        SDL_Texture* playerScoreTexture = renderText(playerScoreText, textColor);
        SDL_Texture* opponentScoreTexture = renderText(opponentScoreText, textColor);

        renderTexture(playerScoreTexture, SCREEN_WIDTH / 2 - 100, 10);
        renderTexture(opponentScoreTexture, SCREEN_WIDTH / 2 + 10, 10);

        SDL_DestroyTexture(playerScoreTexture);
        SDL_DestroyTexture(opponentScoreTexture);

        SDL_RenderPresent(gRenderer);

    }
    delete ball;
    delete playerPaddle;
    delete opponentPaddle;
    close();
    return 0;
}


