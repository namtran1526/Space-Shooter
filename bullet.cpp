#include "bullet.h"
#include <SDL_image.h>
#include <iostream>
#include <algorithm>

// Constructor khởi tạo Bullet
BulletManager::BulletManager(SDL_Renderer* renderer, SoundManager* sound) : texture(nullptr), lastShotTime(0), soundManager(sound) {
    SDL_Surface* bulletSurface = IMG_Load("resources/Bullet.png");
    if (bulletSurface) {
        texture = SDL_CreateTextureFromSurface(renderer, bulletSurface);
        SDL_FreeSurface(bulletSurface);
    } else {
        std::cout << "Warning: Could not load Bullet.png! Using fallback. " << IMG_GetError() << std::endl;
    }
}

// Destructor giải phóng Bullet
BulletManager::~BulletManager() {
    if (texture) SDL_DestroyTexture(texture);
}

// Cập nhật vị trí Bullet, kiểm tra đã đi ra khỏi
void BulletManager::update(ScoreManager& scoreManager) {
    for (auto& bullet : bullets) {
        if (bullet.active) {
            bullet.rect.y -= BULLET_SPEED;
            if (bullet.rect.y < 0) {
                bullet.active = false;
            }
        }
    }
    bullets.erase(std::remove_if(bullets.begin(), bullets.end(), [](const GameObject& obj) { return !obj.active; }), bullets.end()); // Xóa các viên đạn không còn hoạt động
}

// Được bắn đạn ra nếu hết thời gian cooldown
void BulletManager::shoot(int x, int y, Uint32 currentTime, Uint32 cooldown) {
    if (currentTime - lastShotTime > cooldown) {
        bullets.push_back({{x - BULLET_WIDTH/2, y - BULLET_HEIGHT, BULLET_WIDTH, BULLET_HEIGHT}, true, texture}); // Tạo viên đạn mới
        lastShotTime = currentTime;
        soundManager->playShootSound(); // Phát âm thanh bắn, đã kiểm tra musicOn
    }
}

// Render tất cả các viên đạn đang hoạt động 
void BulletManager::render(SDL_Renderer* renderer) {
    for (const auto& bullet : bullets) {
        if (bullet.active) {
            if (bullet.texture) {
                SDL_RenderCopy(renderer, bullet.texture, nullptr, &bullet.rect);
            } else {
                SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
                SDL_RenderFillRect(renderer, &bullet.rect);
            }
        }
    }
}
