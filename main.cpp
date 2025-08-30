#include <SDL.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>

using namespace std;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

const int PADDLE_WIDTH = 20;
const int PADDLE_HEIGHT = 100;

const int BALL_SIZE = 20;

int main(int argc, char* argv[]) {
    srand((unsigned int)time(0));

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        cerr << "SDL Init failed: " << SDL_GetError() << endl;
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow(
        "SDL2 Pong",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH, SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN
    );

    if (!window) {
        cerr << "Window creation failed: " << SDL_GetError() << endl;
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        cerr << "Renderer creation failed: " << SDL_GetError() << endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    
    SDL_Rect leftPaddle = { 50, SCREEN_HEIGHT / 2 - PADDLE_HEIGHT / 2, PADDLE_WIDTH, PADDLE_HEIGHT };
    SDL_Rect rightPaddle = { SCREEN_WIDTH - 70, SCREEN_HEIGHT / 2 - PADDLE_HEIGHT / 2, PADDLE_WIDTH, PADDLE_HEIGHT };

    
    SDL_Rect ball = { SCREEN_WIDTH / 2 - BALL_SIZE / 2, SCREEN_HEIGHT / 2 - BALL_SIZE / 2, BALL_SIZE, BALL_SIZE };
    int ballVelX = (rand() % 2 == 0) ? 5 : -5;
    int ballVelY = (rand() % 2 == 0) ? 5 : -5;

    const int paddleSpeed = 8;


    int leftScore = 0;
    int rightScore = 0;

    bool running = true;
    SDL_Event e;

    while (running) {
        
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT)
                running = false;
        }

        const Uint8* keystates = SDL_GetKeyboardState(NULL);

        // Player control LEFT
        if (keystates[SDL_SCANCODE_UP] && leftPaddle.y > 0)
            leftPaddle.y -= paddleSpeed;
        if (keystates[SDL_SCANCODE_DOWN] && leftPaddle.y < SCREEN_HEIGHT - PADDLE_HEIGHT)
            leftPaddle.y += paddleSpeed;

        // AI RIGHT
        if (ball.y < rightPaddle.y && rightPaddle.y > 0)
            rightPaddle.y -= paddleSpeed;
        if (ball.y > rightPaddle.y + PADDLE_HEIGHT && rightPaddle.y < SCREEN_HEIGHT - PADDLE_HEIGHT)
            rightPaddle.y += paddleSpeed;

        ball.x += ballVelX;
        ball.y += ballVelY;

        if (ball.y <= 0 || ball.y >= SCREEN_HEIGHT - BALL_SIZE)
            ballVelY = -ballVelY;

        if (SDL_HasIntersection(&ball, &leftPaddle) || SDL_HasIntersection(&ball, &rightPaddle)) {
            ballVelX = -ballVelX;

            if (ballVelX > 0) ballVelX++;
            else ballVelX--;
            if (ballVelY > 0) ballVelY++;
            else ballVelY--;
        }

        
        if (ball.x < 0) { // Right player score
            rightScore++;
            ball.x = SCREEN_WIDTH / 2 - BALL_SIZE / 2;
            ball.y = SCREEN_HEIGHT / 2 - BALL_SIZE / 2;
            ballVelX = (rand() % 2 == 0) ? 5 : -5;
            ballVelY = (rand() % 2 == 0) ? 5 : -5;
        }
        else if (ball.x > SCREEN_WIDTH) { // Left player score
            leftScore++;
            ball.x = SCREEN_WIDTH / 2 - BALL_SIZE / 2;
            ball.y = SCREEN_HEIGHT / 2 - BALL_SIZE / 2;
            ballVelX = (rand() % 2 == 0) ? 5 : -5;
            ballVelY = (rand() % 2 == 0) ? 5 : -5;
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &leftPaddle);
        SDL_RenderFillRect(renderer, &rightPaddle);
        SDL_RenderFillRect(renderer, &ball);

        // Print score
        cout << "Score: Left = " << leftScore << " | Right = " << rightScore << "\r" << flush;

        SDL_RenderPresent(renderer);

        SDL_Delay(16); 
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
