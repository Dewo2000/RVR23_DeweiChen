#include "Paddle.h"
#include "Ball.h"
#include <SDL2/SDL.h>

Paddle::Paddle(int screenWidth, int screenHeight, int width, int height, bool isPlayer)
{
    this->screenWidth = screenWidth;
    this->screenHeight = screenHeight;
    this->width = width;
    this->height = height;
    this->isPlayer = isPlayer;

    x = screenWidth / 2 - width / 2;
    y = isPlayer ? screenHeight - height - 10 : 10;
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
