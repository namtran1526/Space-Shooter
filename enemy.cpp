#include "enemy.h"
#include "score.h"
#include <SDL_image.h>
#include <iostream>
#include <algorithm>
#include <cstdlib>

// Constructor khởi tạo Enemy
EnemyManager::EnemyManager(SDL_Renderer* renderer) : texture(nullptr), lastSpawnTime(0) {
    SDL_Surface* enemySurface = IMG_Load("resources/Enemy.png");
    if (enemySurface) {
        texture = SDL_CreateTextureFromSurface(renderer, enemySurface);
        SDL_FreeSurface(enemySurface);
    } else {
        std::cout << "Warning: Could not load Enemy.png! Using fallback. " << IMG_GetError() << std::endl;
    }
}

// Destructor giải phóng Enemy
EnemyManager::~EnemyManager() {
    if (texture) SDL_DestroyTexture(texture);
}

// Cập nhật vị trí Enemy, kiểm tra va chạm và di chuyển xuống dưới
void EnemyManager::update(ScoreManager& scoreManager, const GameObject& player) {
    for (auto& enemy : enemies) {
        if (enemy.active) {
            enemy.rect.y += ENEMY_SPEED;
            if (enemy.rect.y > WINDOW_HEIGHT) {
                enemy.active = false;
            } else if (player.active && SDL_HasIntersection(&player.rect, &enemy.rect)) { // Nếu va chạm với player sẽ giảm mạng của Player
                scoreManager.loseLife();
                enemy.active = false;
            }
        }
    }
    enemies.erase(std::remove_if(enemies.begin(), enemies.end(), [](const GameObject& obj) { return !obj.active; }), enemies.end());
}

// Sinh ra Enemy mới dựa trên thời gian hiện tại
void EnemyManager::spawn(Uint32 currentTime) { 
    if (currentTime - lastSpawnTime > SPAWN_INTERVAL) {
        enemies.push_back({{rand() % (WINDOW_WIDTH - ENEMY_WIDTH), 0, ENEMY_WIDTH, ENEMY_HEIGHT}, true, texture}); // Sinh ra enemy mới với vị trí ngẫu nhiên
        lastSpawnTime = currentTime;
    }
}

// Render tất cả các Enemy đang hoạt động
void EnemyManager::render(SDL_Renderer* renderer) {
    for (const auto& enemy : enemies) {
        if (enemy.active) {
            if (enemy.texture) {
                SDL_RenderCopy(renderer, enemy.texture, nullptr, &enemy.rect);
            } else {
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
                SDL_RenderFillRect(renderer, &enemy.rect);
            }
        }
    }
}
