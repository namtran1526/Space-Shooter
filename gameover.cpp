#include "gameover.h"
#include "score.h"
#include "player.h"
#include "bullet.h"
#include "enemy.h"

// Constructor khởi tạo GameOver
GameOverManager::GameOverManager(SDL_Renderer* renderer, SoundManager* sound) : backgroundTexture(nullptr), homeButtonTexture(nullptr), resetButtonTexture(nullptr), soundManager(sound) {
    loadTextures(renderer);
}

// Destructor giải phóng GameOver
GameOverManager::~GameOverManager() {
    if (backgroundTexture) SDL_DestroyTexture(backgroundTexture);
    if (homeButtonTexture) SDL_DestroyTexture(homeButtonTexture);
    if (resetButtonTexture) SDL_DestroyTexture(resetButtonTexture);
}

// Hàm load các texture từ file
void GameOverManager::loadTextures(SDL_Renderer* renderer) {
    SDL_Surface* bgSurface = IMG_Load("resources/GameOver_BG.png");
    if (bgSurface) {
        backgroundTexture = SDL_CreateTextureFromSurface(renderer, bgSurface);
        SDL_FreeSurface(bgSurface);
    }
    SDL_Surface* homeSurface = IMG_Load("resources/Home_Button.png");
    if (homeSurface) {
        homeButtonTexture = SDL_CreateTextureFromSurface(renderer, homeSurface);

        SDL_FreeSurface(homeSurface);
    }
    SDL_Surface* resetSurface = IMG_Load("resources/Reset_Button.png");
    if (resetSurface) {
        resetButtonTexture = SDL_CreateTextureFromSurface(renderer, resetSurface);
        SDL_FreeSurface(resetSurface);
    }
}

// Render giao diện Game Over
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

// Hàm xử lý sự kiện click chuột
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
