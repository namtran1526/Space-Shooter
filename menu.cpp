#include "menu.h"

MenuManager::MenuManager(SDL_Renderer* renderer, SoundManager* sound) : backgroundTexture(nullptr), playButtonTexture(nullptr), instructionButtonTexture(nullptr), musicButtonTexture(nullptr), pauseButtonTexture(nullptr), musicOn(true), soundManager(sound) {
    musicButtonSrcRect = {0, 0, 100, 100};
    pauseButtonSrcRect = {0, 0, 50, 50};
    loadTextures(renderer);
    soundManager->toggleMusic(musicOn, MENU); // Khởi tạo trạng thái nhạc
}

MenuManager::~MenuManager() {
    if (backgroundTexture) SDL_DestroyTexture(backgroundTexture);
    if (playButtonTexture) SDL_DestroyTexture(playButtonTexture);
    if (instructionButtonTexture) SDL_DestroyTexture(instructionButtonTexture);
    if (musicButtonTexture) SDL_DestroyTexture(musicButtonTexture);
    if (pauseButtonTexture) SDL_DestroyTexture(pauseButtonTexture);
}

void MenuManager::loadTextures(SDL_Renderer* renderer) {
    SDL_Surface* backgroundSurface = IMG_Load("resources/Menu_BG.png");
    if (backgroundSurface) {
        backgroundTexture = SDL_CreateTextureFromSurface(renderer, backgroundSurface);
        SDL_FreeSurface(backgroundSurface);
    } else {
        std::cout << "Error: Could not load Menu_BG.png! " << IMG_GetError() << std::endl;
    }

    SDL_Surface* playSurface = IMG_Load("resources/Play_button.png");
    if (playSurface) {
        playButtonTexture = SDL_CreateTextureFromSurface(renderer, playSurface);
        SDL_FreeSurface(playSurface);
    } else {
        std::cout << "Warning: Could not load Play_button.png! " << IMG_GetError() << std::endl;
    }

    SDL_Surface* instructionSurface = IMG_Load("resources/Instruction_button.png");
    if (instructionSurface) {
        instructionButtonTexture = SDL_CreateTextureFromSurface(renderer, instructionSurface);
        SDL_FreeSurface(instructionSurface);
    } else {
        std::cout << "Warning: Could not load Instruction_button.png! " << IMG_GetError() << std::endl;
    }

    SDL_Surface* musicSurface = IMG_Load("resources/Music_button.png");
    if (musicSurface) {
        musicButtonTexture = SDL_CreateTextureFromSurface(renderer, musicSurface);
        SDL_FreeSurface(musicSurface);
    } else {
        std::cout << "Warning: Could not load Music_button.png! " << IMG_GetError() << std::endl;
    }

    SDL_Surface* pauseSurface = IMG_Load("resources/Pause_Button.png");
    if (pauseSurface) {
        pauseButtonTexture = SDL_CreateTextureFromSurface(renderer, pauseSurface);
        SDL_FreeSurface(pauseSurface);
    } else {
        std::cout << "Warning: Could not load Pause_Button.png! " << IMG_GetError() << std::endl;
    }
}

void MenuManager::render(SDL_Renderer* renderer, GameState state) {
    if (state == MENU) {
        if (backgroundTexture) {
            SDL_RenderCopy(renderer, backgroundTexture, nullptr, nullptr);
        } else {
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);
            std::cout << "Warning: Menu background not loaded, using black background!" << std::endl;
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
            std::cout << "Pause button rendered at (" << pauseRect.x << "," << pauseRect.y << ") for state: " << (state == PLAYING ? "PLAYING" : "PAUSED") << std::endl;
        } else {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_Rect pauseRect = {WINDOW_WIDTH - 60, 10, 50, 50};
            SDL_RenderFillRect(renderer, &pauseRect);
            std::cout << "Pause button texture not loaded, using white rectangle at (" << pauseRect.x << "," << pauseRect.y << ")" << std::endl;
        }
    }
}

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
    } else if (state == PLAYING || state == PAUSED) {
        SDL_Rect pauseRect = {WINDOW_WIDTH - 60, 10, 50, 50};
        if (x >= pauseRect.x && x <= pauseRect.x + pauseRect.w && y >= pauseRect.y && y <= pauseRect.y + pauseRect.h) {
            pauseButtonSrcRect.x = (state == PLAYING) ? 50 : 0;
            state = (state == PLAYING) ? PAUSED : PLAYING;
            std::cout << (state == PAUSED ? "Game Paused" : "Game Resumed") << " at click (" << x << "," << y << ")" << std::endl;
        }
    }
}
