#ifndef PADDLE_H
#define PADDLE_H

#include "Serializable.h"

    
class SDL_Renderer;
class Ball;
class Paddle : public Serializable
{
public:

    Paddle(){   };
    Paddle(int screenWidth, int screenHeight, int width, int height, bool isPlayer,int x ,int y);

    void handleInput();
    void update();
    void updateAI(Ball* ball);
    void render(SDL_Renderer* renderer);

    int getX() const { return x; }
    int getY() const { return y; }
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    void setPos(int x, int y );
    void to_bin();
    int from_bin(char * bobj);

private:
    int x;
    int y;
    int velocity;
    int screenWidth;
    int screenHeight;
    int width;
    int height;
    bool isPlayer;
};

#endif
