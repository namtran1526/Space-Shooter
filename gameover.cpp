#include "gameover.h"
#include "score.h"
#include "player.h"
#include "bullet.h"
#include "enemy.h"

GameOverManager::GameOverManager(SDL_Renderer* renderer, SoundManager* sound) : backgroundTexture(nullptr), homeButtonTexture(nullptr), resetButtonTexture(nullptr), soundManager(sound) {
    loadTextures(renderer);
}

GameOverManager::~GameOverManager() {
    if (backgroundTexture) SDL_DestroyTexture(backgroundTexture);
    if (homeButtonTexture) SDL_DestroyTexture(homeButtonTexture);
    if (resetButtonTexture) SDL_DestroyTexture(resetButtonTexture);
}

void GameOverManager::loadTextures(SDL_Renderer* renderer) {
    SDL_Surface* bgSurface = IMG_Load("resources/GameOver_BG.png");
    if (bgSurface) {
        backgroundTexture = SDL_CreateTextureFromSurface(renderer, bgSurface);
        if (!backgroundTexture) {
            std::cout << "Error: Could not create texture from GameOver_BG.png! " << SDL_GetError() << std::endl;
        }
        SDL_FreeSurface(bgSurface);
    } else {
        std::cout << "Warning: Could not load GameOver_BG.png! " << IMG_GetError() << std::endl;
    }

    SDL_Surface* homeSurface = IMG_Load("resources/Home_Button.png");
    if (homeSurface) {
        homeButtonTexture = SDL_CreateTextureFromSurface(renderer, homeSurface);
        if (!homeButtonTexture) {
            std::cout << "Error: Could not create texture from Home_Button.png! " << SDL_GetError() << std::endl;
        }
        SDL_FreeSurface(homeSurface);
    } else {
        std::cout << "Warning: Could not load Home_Button.png! " << IMG_GetError() << std::endl;
    }

    SDL_Surface* resetSurface = IMG_Load("resources/Reset_Button.png");
    if (resetSurface) {
        resetButtonTexture = SDL_CreateTextureFromSurface(renderer, resetSurface);
        if (!resetButtonTexture) {
            std::cout << "Error: Could not create texture from Reset_Button.png! " << SDL_GetError() << std::endl;
        }
        SDL_FreeSurface(resetSurface);
    } else {
        std::cout << "Warning: Could not load Reset_Button.png! " << IMG_GetError() << std::endl;
    }
}

void GameOverManager::render(SDL_Renderer* renderer) {
    if (backgroundTexture) {
        SDL_RenderCopy(renderer, backgroundTexture, nullptr, nullptr);
    } else {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        std::cout << "Warning: GameOver background not loaded, using black background!" << std::endl;
    }

    if (homeButtonTexture) {
        SDL_Rect homeRect = {250, 400, 100, 100};
        SDL_RenderCopy(renderer, homeButtonTexture, nullptr, &homeRect);
    } else {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_Rect homeRect = {250, 400, 100, 100};
        SDL_RenderFillRect(renderer, &homeRect);
    }

    if (resetButtonTexture) {
        SDL_Rect resetRect = {450, 400, 100, 100};
        SDL_RenderCopy(renderer, resetButtonTexture, nullptr, &resetRect);
    } else {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_Rect resetRect = {450, 400, 100, 100};
        SDL_RenderFillRect(renderer, &resetRect);
    }
}

void GameOverManager::handleClick(int x, int y, GameState& state, ScoreManager& scoreManager, Player& player, BulletManager& bulbs, EnemyManager& enemies, Uint32& countdownStart, int& countdownStep, Uint32& lastEnemySpawn) {
    SDL_Rect homeRect = {250, 400, 100, 100};
    SDL_Rect resetRect = {450, 400, 100, 100};

    if (x >= homeRect.x && x <= homeRect.x + homeRect.w && y >= homeRect.y && y <= homeRect.y + homeRect.h) {
        state = MENU;
        soundManager->playMusicForState(MENU); // Phát nhạc Menu
        std::cout << "Clicked Home button, transitioning to MENU" << std::endl;
    } else if (x >= resetRect.x && x <= resetRect.x + resetRect.w && y >= resetRect.y && y <= resetRect.y + resetRect.h) {
        state = COUNTDOWN;
        scoreManager.reset();
        player.resetPosition();
        bulbs.getBullets().clear();
        enemies.getEnemies().clear();
        countdownStart = SDL_GetTicks();
        countdownStep = 0;
        lastEnemySpawn = 0;
        soundManager->playMusicForState(COUNTDOWN); // Phát nhạc Countdown
        std::cout << "Clicked Reset button, transitioning to COUNTDOWN" << std::endl;
    }
}
