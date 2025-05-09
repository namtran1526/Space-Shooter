#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include <vector>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const int PLAYER_WIDTH = 80;
const int PLAYER_HEIGHT = 80;
const int BULLET_WIDTH = 10;
const int BULLET_HEIGHT = 30;
const int ENEMY_WIDTH = 50;
const int ENEMY_HEIGHT = 50;
const int PLAYER_SPEED = 5;
const int BULLET_SPEED = 10;
const int ENEMY_SPEED = 10;
const Uint32 BULLET_COOLDOWN = 500;
const Uint32 COUNTDOWN_DELAY = 1000;
const Uint32 EXPLOSION_FRAME_DELAY = 250;

enum GameState {
    MENU,
    INSTRUCTIONS,
    COUNTDOWN,
    PLAYING,
    PAUSED,
    EXPLODING,
    GAME_OVER
};

struct GameObject {
    SDL_Rect rect;
    bool active;
    SDL_Texture* texture;
};

struct Explosion {
    SDL_Rect position;
    std::vector<SDL_Rect> frames;
    int currentFrame;
    Uint32 lastFrameTime;
    bool active;
};

#endif
