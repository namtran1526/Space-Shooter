#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include <vector>

// Kích thước cửa sổ game
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

// Kích thước và tốc độ Player
const int PLAYER_WIDTH = 80;
const int PLAYER_HEIGHT = 80;
const int PLAYER_SPEED = 5;

// Kích thước và tốc độ đạn
const int BULLET_WIDTH = 10;
const int BULLET_HEIGHT = 30;
const int BULLET_SPEED = 10;

// Kích thước và tốc độ thiên thạch
const int ENEMY_WIDTH = 50;
const int ENEMY_HEIGHT = 50;
const int ENEMY_SPEED = 10;

const Uint32 BULLET_COOLDOWN = 500; // Thời gian chờ bắn đạn
const Uint32 COUNTDOWN_DELAY = 1000; // Thời gian 1s mỗi bước đếm ngược
const Uint32 EXPLOSION_FRAME_DELAY = 250; // Thời gian chờ hiệu ứng nổ

// Định nghĩa trạng thái trò chơi
enum GameState {
    MENU,
    INSTRUCTIONS,
    COUNTDOWN,
    PLAYING,
    PAUSED,
    EXPLODING,
    GAME_OVER
};

// Cấu trúc đối tượng trò chơi (Player, Bullet, Enemy)
struct GameObject {
    SDL_Rect rect; // hình chữ nhật xác định vị trí và kích thước
    bool active; // Kiểm tra đối tượng hoạt động không
    SDL_Texture* texture; // Texture của đối tượng để hiển thị lên màn hình
};

struct Explosion {
    SDL_Rect position; // vị trí vụ nổ
    std::vector<SDL_Rect> frames; // các khung hình tạo hiệu ứng nổ
    int currentFrame; // khung hình hiện tại
    Uint32 lastFrameTime; // thời điểm cập nhật khung hình cuối cùng
    bool active; // xác định vụ nổ có xảy ra không
};

#endif
