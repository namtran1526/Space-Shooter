#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <vector>

// Window dimensions
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

// Player properties
const int PLAYER_WIDTH = 50;
const int PLAYER_HEIGHT = 50;
const int PLAYER_SPEED = 5;

// Bullet properties
const int BULLET_WIDTH = 5;
const int BULLET_HEIGHT = 10;
const int BULLET_SPEED = 10;
const Uint32 BULLET_COOLDOWN = 500; // 500ms cooldown between shots

// Enemy properties
const int ENEMY_WIDTH = 40;
const int ENEMY_HEIGHT = 40;
const int ENEMY_SPEED = 3;

// Collision counter
const int MAX_COLLISIONS = 3;

struct GameObject {
    SDL_Rect rect;
    bool active;
};

#endif
