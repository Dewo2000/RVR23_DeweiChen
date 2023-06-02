#include "Ball.h"
#include "Paddle.h"
#include <SDL2/SDL.h>
Ball::Ball(int screenWidth, int screenHeight)
{
    this->screenWidth = screenWidth;
    this->screenHeight = screenHeight;

    width = 10;
    height = 10;

    x = screenWidth / 2 - width / 2;
    y = screenHeight / 2 - height / 2;
                velocityX = 2;
    velocityY = 2;
}

void Ball::update(Paddle* playerPaddle, Paddle* opponentPaddle, int& playerScore, int& opponentScore)
{
    x += velocityX;
    y += velocityY;

    if (x <= 0 || x >= screenWidth - width)
    {
        velocityX = -velocityX;
    }

    if (y <= 0)
    {
        x = screenWidth / 2 - width / 2;
        y = screenHeight / 2 - height / 2;
        opponentScore++;
    }
    else if (y >= screenHeight - height)
    {
        x = screenWidth / 2 - width / 2;
        y = screenHeight / 2 - height / 2;
        playerScore++;
    }

    if (y + height >= playerPaddle->getY() && y <= playerPaddle->getY() + playerPaddle->getHeight() && x + width >= playerPaddle->getX() && x <= playerPaddle->getX() + playerPaddle->getWidth())
    {
        velocityY = -velocityY;
    }

    if (y <= opponentPaddle->getY() + opponentPaddle->getHeight() && y + height >= opponentPaddle->getY() && x + width >= opponentPaddle->getX() && x <= opponentPaddle->getX() + opponentPaddle->getWidth())
    {
        velocityY = -velocityY;
    }
}

void Ball::render(SDL_Renderer* renderer)
{
    SDL_Rect rect = { x, y, width, height };
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderFillRect(renderer, &rect);
}

void Ball::to_bin(){

    alloc_data(2*sizeof(int));
    memset(_data, 0, sizeof(int));
    char *tmp = _data;
    memcpy(tmp,&x,sizeof(int));
    tmp+=sizeof(int);
    memcpy(tmp,&y,sizeof(int));

}
int Ball::from_bin(char * bobj){
    alloc_data(2*sizeof(int));
    memcpy(static_cast<void *>(_data), bobj, 2*sizeof(int));
    char *tmp = _data;
    memcpy(&x,tmp,sizeof(int));
    tmp+=sizeof(int);
    memcpy(&y,tmp,sizeof(int));
    return 0;
}