#ifndef GAMEOVER_H
#define GAMEOVER_H

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include "game.h"
#include "score.h"
#include "player.h"
#include "bullet.h"
#include "enemy.h"
#include "sound.h"

// Lớp GameOverManager quản lý giao diện Game Over
class GameOverManager {
private:
    SDL_Texture* backgroundTexture; // Texture nền Game Over
    SDL_Texture* homeButtonTexture; // Texture nút về trang chủ
    SDL_Texture* resetButtonTexture; // Texture nút reset
    SoundManager* soundManager; // Quản lý âm thanh
    void loadTextures(SDL_Renderer* renderer); // Load các hình ảnh nút bấm

public:
    GameOverManager(SDL_Renderer* renderer, SoundManager* sound); // Constructor khởi tạo GameOverManager
    ~GameOverManager(); // Destructor giải phóng GameOverManager
    void render(SDL_Renderer* renderer); // Render giao diện Game Over

    // Hàm xử lý sự kiện click chuột
    void handleClick(int x, int y, GameState& state, ScoreManager& scoreManager, Player& player, BulletManager& bullets, EnemyManager& enemies, Uint32& countdownStart, int& countdownStep, Uint32& lastEnemySpawn);
};

#endif
