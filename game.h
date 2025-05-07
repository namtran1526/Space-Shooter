#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include <vector>

// Hằng số cho kích thước cửa sổ và các đối tượng
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const int PLAYER_WIDTH = 80;
const int PLAYER_HEIGHT = 80;
const int BULLET_WIDTH = 10;
const int BULLET_HEIGHT = 30;
const int ENEMY_WIDTH = 40;
const int ENEMY_HEIGHT = 40;
const int PLAYER_SPEED = 5;
const int BULLET_SPEED = 10;
const int ENEMY_SPEED = 3;
const Uint32 BULLET_COOLDOWN = 500; // 500ms giữa các lần bắn
const Uint32 COUNTDOWN_DELAY = 1000; // 1 giây cho mỗi bước đếm

// Enum cho trạng thái game
enum GameState {
    MENU,
    INSTRUCTIONS,
    COUNTDOWN,
    PLAYING,
    GAME_OVER
};

// Cấu trúc GameObject để đại diện cho player, bullet, enemy
struct GameObject {
    SDL_Rect rect;
    bool active;
    SDL_Texture* texture;
};

#endif
