#ifndef PADDLE_H
#define PADDLE_H

class SDL_Renderer;
class Ball;
class Paddle
{
public:
    Paddle(int screenWidth, int screenHeight, int width, int height, bool isPlayer);

    void handleInput();
    void update();
    void updateAI(Ball* ball);
    void render(SDL_Renderer* renderer);

    int getX() const { return x; }
    int getY() const { return y; }
    int getWidth() const { return width; }
    int getHeight() const { return height; }

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
