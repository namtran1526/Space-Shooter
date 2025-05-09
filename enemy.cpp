#include "enemy.h"
#include "score.h"
#include <SDL_image.h>
#include <iostream>
#include <algorithm>
#include <cstdlib>

EnemyManager::EnemyManager(SDL_Renderer* renderer) : texture(nullptr), lastSpawnTime(0) {
    SDL_Surface* enemySurface = IMG_Load("resources/Enemy.png");
    if (enemySurface) {
        texture = SDL_CreateTextureFromSurface(renderer, enemySurface);
        SDL_FreeSurface(enemySurface);
    } else {
        std::cout << "Warning: Could not load Enemy.png! Using fallback. " << IMG_GetError() << std::endl;
    }
}

EnemyManager::~EnemyManager() {
    if (texture) SDL_DestroyTexture(texture);
}

void EnemyManager::update(ScoreManager& scoreManager, const GameObject& player) {
    for (auto& enemy : enemies) {
        if (enemy.active) {
            enemy.rect.y += ENEMY_SPEED;
            if (enemy.rect.y > WINDOW_HEIGHT) {
                enemy.active = false;
                std::cout << "Enemy removed: Out of screen at y=" << enemy.rect.y << std::endl;
            } else if (player.active && SDL_HasIntersection(&player.rect, &enemy.rect)) {
                scoreManager.loseLife();
                enemy.active = false;
                std::cout << "Collision: Player hit by enemy, lives left: " << scoreManager.getLives() << std::endl;
            }
        }
    }
    enemies.erase(std::remove_if(enemies.begin(), enemies.end(), [](const GameObject& obj) { return !obj.active; }), enemies.end());
}

void EnemyManager::spawn(Uint32 currentTime) {
    if (currentTime - lastSpawnTime > SPAWN_INTERVAL) {
        enemies.push_back({{rand() % (WINDOW_WIDTH - ENEMY_WIDTH), 0, ENEMY_WIDTH, ENEMY_HEIGHT}, true, texture});
        lastSpawnTime = currentTime;
        std::cout << "Enemy spawned at x=" << enemies.back().rect.x << std::endl;
    }
}

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
