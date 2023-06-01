#ifndef BALL_H
#define BALL_H


class SDL_Renderer;
class Paddle;
class Ball
{
public:
    Ball(int screenWidth, int screenHeight);

    void update(Paddle* playerPaddle, Paddle* opponentPaddle, int& playerScore, int& opponentScore);
    void render(SDL_Renderer* renderer);
    bool checkCollision();
    int getX() const { return x; }
    int getY() const { return y; }
    int getWidth() const { return width; }
    int getHeight() const { return height; }

private:
    int x;
    int y;
    int velocityX;
    int velocityY;
    int screenWidth;
    int screenHeight;
    int width;
    int height;
};

#endif
