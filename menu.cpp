#include "menu.h"

// Constructor khởi tạo Menu
MenuManager::MenuManager(SDL_Renderer* renderer, SoundManager* sound) : backgroundTexture(nullptr), playButtonTexture(nullptr), instructionButtonTexture(nullptr), musicButtonTexture(nullptr), pauseButtonTexture(nullptr), musicOn(true), soundManager(sound) {
    musicButtonSrcRect = {0, 0, 100, 100}; // nút âm thanh
    pauseButtonSrcRect = {0, 0, 50, 50}; // nút tạm dừng
    loadTextures(renderer);
    soundManager->toggleMusic(musicOn, MENU); // Khởi tạo trạng thái nhạc
}

// Destructor giải phóng Menu
MenuManager::~MenuManager() {
    if (backgroundTexture) SDL_DestroyTexture(backgroundTexture);
    if (playButtonTexture) SDL_DestroyTexture(playButtonTexture);
    if (instructionButtonTexture) SDL_DestroyTexture(instructionButtonTexture);
    if (musicButtonTexture) SDL_DestroyTexture(musicButtonTexture);
    if (pauseButtonTexture) SDL_DestroyTexture(pauseButtonTexture);
}

// Hàm load các texture từ file
void MenuManager::loadTextures(SDL_Renderer* renderer) {
    SDL_Surface* backgroundSurface = IMG_Load("resources/Menu_BG.png");
    if (backgroundSurface) {
        backgroundTexture = SDL_CreateTextureFromSurface(renderer, backgroundSurface);
        SDL_FreeSurface(backgroundSurface);
    }
    SDL_Surface* playSurface = IMG_Load("resources/Play_button.png");
    if (playSurface) {
        playButtonTexture = SDL_CreateTextureFromSurface(renderer, playSurface);
        SDL_FreeSurface(playSurface);
    }
    SDL_Surface* instructionSurface = IMG_Load("resources/Instruction_button.png");
    if (instructionSurface) {
        instructionButtonTexture = SDL_CreateTextureFromSurface(renderer, instructionSurface);
        SDL_FreeSurface(instructionSurface);
    }
    SDL_Surface* musicSurface = IMG_Load("resources/Music_button.png");
    if (musicSurface) {
        musicButtonTexture = SDL_CreateTextureFromSurface(renderer, musicSurface);
        SDL_FreeSurface(musicSurface);
    }
    SDL_Surface* pauseSurface = IMG_Load("resources/Pause_Button.png");
    if (pauseSurface) {
        pauseButtonTexture = SDL_CreateTextureFromSurface(renderer, pauseSurface);
        SDL_FreeSurface(pauseSurface);
    }
}

// Render các nút và nền menu
void MenuManager::render(SDL_Renderer* renderer, GameState state) {
    if (state == MENU) {
        if (backgroundTexture) {
            SDL_RenderCopy(renderer, backgroundTexture, nullptr, nullptr);
        }
        if (playButtonTexture) {
            SDL_Rect playRect = {350, 450, 100, 100};
            SDL_RenderCopy(renderer, playButtonTexture, nullptr, &playRect);
        }

        if (instructionButtonTexture) {
            SDL_Rect instructionRect = {200, 450, 100, 100};
            SDL_RenderCopy(renderer, instructionButtonTexture, nullptr, &instructionRect);
        }

        if (musicButtonTexture) {
            SDL_Rect musicRect = {500, 450, 100, 100};
            SDL_RenderCopy(renderer, musicButtonTexture, &musicButtonSrcRect, &musicRect);
        }
    } else if (state == PLAYING || state == PAUSED) {
        if (pauseButtonTexture) {
            SDL_Rect pauseRect = {WINDOW_WIDTH - 60, 10, 50, 50};
            SDL_RenderCopy(renderer, pauseButtonTexture, &pauseButtonSrcRect, &pauseRect);
        } else {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_Rect pauseRect = {WINDOW_WIDTH - 60, 10, 50, 50};
            SDL_RenderFillRect(renderer, &pauseRect);
        }
    }
}

// Hàm xử lý sự kiện click chuột
void MenuManager::handleClick(int x, int y, GameState& state) {
    if (state == MENU) {
        SDL_Rect playRect = {350, 450, 100, 100};
        SDL_Rect instructionRect = {200, 450, 100, 100};
        SDL_Rect musicRect = {500, 450, 100, 100};

        if (x >= playRect.x && x <= playRect.x + playRect.w && y >= playRect.y && y <= playRect.y + playRect.h) {
            state = COUNTDOWN;
            std::cout << "Clicked Play button, transitioning to COUNTDOWN" << std::endl;
        } else if (x >= instructionRect.x && x <= instructionRect.x + instructionRect.w && y >= instructionRect.y && y <= instructionRect.y + instructionRect.h) {
            state = INSTRUCTIONS;
            std::cout << "Clicked Instruction button, transitioning to INSTRUCTIONS" << std::endl;
        } else if (x >= musicRect.x && x <= musicRect.x + musicRect.w && y >= musicRect.y && y <= musicRect.y + musicRect.h) {
            musicOn = !musicOn;
            musicButtonSrcRect.x = musicOn ? 0 : 100;
            soundManager->toggleMusic(musicOn, MENU); // Truyền trạng thái MENU
            std::cout << (musicOn ? "Music On" : "Music Off") << std::endl;
        }
    } else if (state == PLAYING || state == PAUSED) { // Tạm dừng hoặc chơi
        SDL_Rect pauseRect = {WINDOW_WIDTH - 60, 10, 50, 50};
        if (x >= pauseRect.x && x <= pauseRect.x + pauseRect.w && y >= pauseRect.y && y <= pauseRect.y + pauseRect.h) {
            pauseButtonSrcRect.x = (state == PLAYING) ? 50 : 0;
            state = (state == PLAYING) ? PAUSED : PLAYING;
        }
    }
}
