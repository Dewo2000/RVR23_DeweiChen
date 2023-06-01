#include "Paddle.h"
#include "Ball.h"
#include <SDL2/SDL.h>

Paddle::Paddle(int screenWidth, int screenHeight, int width, int height, bool isPlayer , int x ,int y)
{
    this->screenWidth = screenWidth;
    this->screenHeight = screenHeight;
    this->width = width;
    this->height = height;
    this->isPlayer = isPlayer;

    this->x=x;
    this->y=y;
    // x = screenWidth / 2 - width / 2;
    // y = isPlayer ? screenHeight - height - 10 : 10;
    velocity = 5;
}

void Paddle::handleInput()
{
    if (isPlayer)
    {
        const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);

        if (currentKeyStates[SDL_SCANCODE_LEFT])
        {
            x -= velocity;
        }
        if (currentKeyStates[SDL_SCANCODE_RIGHT])
        {
            x += velocity;
        }
    }
}

void Paddle::update()
{
    if (x < 0)
    {
        x = 0;
    }
    else if (x > screenWidth - width)
    {
        x = screenWidth - width;
    }
}

void Paddle::updateAI(Ball* ball)
{
    // Simple AI opponent that follows the ball
    if (!isPlayer)
    {
        if (x + width / 2 < ball->getX() + ball->getWidth() / 2)
        {
            x += velocity;
        }
        else if (x + width / 2 > ball->getX() + ball->getWidth() / 2)
        {
            x -= velocity;
        }
    }
}

void Paddle::render(SDL_Renderer* renderer)
{
    SDL_Rect rect = { x, y, width, height };
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderFillRect(renderer, &rect);
}

void Paddle::to_bin(){

    alloc_data(sizeof(int));
    memset(_data, 0, sizeof(int));
    char *tmp = _data;
    memcpy(tmp,&x,sizeof(int));

}
int Paddle::from_bin(char * bobj){
    alloc_data(sizeof(int));
    memcpy(static_cast<void *>(_data), bobj, sizeof(int));
    char *tmp = _data;
    memcpy(&x,tmp,sizeof(int));
    return 0;
}
