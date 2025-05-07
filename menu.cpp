#include "menu.h"

MenuManager::MenuManager(SDL_Renderer* renderer) : backgroundTexture(nullptr), playButtonTexture(nullptr), instructionButtonTexture(nullptr), musicButtonTexture(nullptr), musicOn(true) {
    musicButtonSrcRect = {0, 0, 100, 100};
    loadTextures(renderer);
}

MenuManager::~MenuManager() {
    if (backgroundTexture) SDL_DestroyTexture(backgroundTexture);
    if (playButtonTexture) SDL_DestroyTexture(playButtonTexture);
    if (instructionButtonTexture) SDL_DestroyTexture(instructionButtonTexture);
    if (musicButtonTexture) SDL_DestroyTexture(musicButtonTexture);
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
}

void MenuManager::render(SDL_Renderer* renderer) {
    if (backgroundTexture) {
        SDL_RenderCopy(renderer, backgroundTexture, nullptr, nullptr);
    } else {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Fallback to black background
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
}

void MenuManager::handleClick(int x, int y, GameState& state) {
    SDL_Rect playRect = {350, 450, 100, 100};
    SDL_Rect instructionRect = {200, 450, 100, 100};
    SDL_Rect musicRect = {500, 450, 100, 100};

    if (x >= playRect.x && x <= playRect.x + playRect.w && y >= playRect.y && y <= playRect.y + playRect.h) {
        state = COUNTDOWN;
    } else if (x >= instructionRect.x && x <= instructionRect.x + instructionRect.w && y >= instructionRect.y && y <= instructionRect.y + instructionRect.h) {
        state = INSTRUCTIONS;
    } else if (x >= musicRect.x && x <= musicRect.x + musicRect.w && y >= musicRect.y && y <= musicRect.y + musicRect.h) {
        musicOn = !musicOn;
        if (musicOn) {
            musicButtonSrcRect = {0, 0, 100, 100};
        } else {
            musicButtonSrcRect = {100, 0, 100, 100};
        }
        std::cout << (musicOn ? "Music On" : "Music Off") << std::endl;
    }
}
