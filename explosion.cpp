#include "explosion.h"
#include <SDL_image.h>
#include <iostream>

ExplosionManager::ExplosionManager(SDL_Renderer* renderer, SoundManager* sound) : texture(nullptr), soundManager(sound) {
    SDL_Surface* explosionSurface = IMG_Load("resources/Boom.png");
    if (explosionSurface) {
        texture = SDL_CreateTextureFromSurface(renderer, explosionSurface);
        SDL_FreeSurface(explosionSurface);
    } else {
        std::cout << "Warning: Could not load Boom.png! " << IMG_GetError() << std::endl;
    }
    std::vector<SDL_Rect> frames;
    if (texture) {
        for (int i = 0; i < 4; ++i) {
            frames.push_back({i * 80, 0, 80, 80});
        }
    }
    explosion = {{0, 0, 80, 80}, frames, 0, 0, false};
}

ExplosionManager::~ExplosionManager() {
    if (texture) SDL_DestroyTexture(texture);
}

void ExplosionManager::update() {
    if (explosion.active) {
        Uint32 currentTime = SDL_GetTicks();
        if (currentTime - explosion.lastFrameTime >= EXPLOSION_FRAME_DELAY) {
            explosion.currentFrame++;
            explosion.lastFrameTime = currentTime;
            std::cout << "Explosion frame: " << explosion.currentFrame << std::endl;
            if (explosion.currentFrame >= static_cast<int>(explosion.frames.size())) {
                explosion.active = false;
                std::cout << "Explosion finished" << std::endl;
            }
        }
    }
}

void ExplosionManager::trigger(int x, int y) {
    explosion.position.x = x - (80 - PLAYER_WIDTH) / 2;
    explosion.position.y = y - (80 - PLAYER_HEIGHT) / 2;
    explosion.currentFrame = 0;
    explosion.lastFrameTime = SDL_GetTicks();
    explosion.active = true;
    soundManager->playBoomSound(); // Phát âm thanh nổ
    std::cout << "Explosion triggered at (" << explosion.position.x << "," << explosion.position.y << ")" << std::endl;
}

void ExplosionManager::render(SDL_Renderer* renderer) {
    if (explosion.active && texture) {
        SDL_RenderCopy(renderer, texture, &explosion.frames[explosion.currentFrame], &explosion.position);
    }
}
