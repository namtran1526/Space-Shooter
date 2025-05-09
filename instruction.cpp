#include "instruction.h"

InstructionManager::InstructionManager(SDL_Renderer* renderer) : backgroundTexture(nullptr), instructionTexture(nullptr), backButtonTexture(nullptr) {
    loadTextures(renderer);
}

InstructionManager::~InstructionManager() {
    if (backgroundTexture) SDL_DestroyTexture(backgroundTexture);
    if (instructionTexture) SDL_DestroyTexture(instructionTexture);
    if (backButtonTexture) SDL_DestroyTexture(backButtonTexture);
}

void InstructionManager::loadTextures(SDL_Renderer* renderer) {
    SDL_Surface* bgSurface = IMG_Load("resources/Play_BG.png");
    if (bgSurface) {
        backgroundTexture = SDL_CreateTextureFromSurface(renderer, bgSurface);
        SDL_FreeSurface(bgSurface);
    } else {
        std::cout << "Warning: Could not load Play_BG.png! " << IMG_GetError() << std::endl;
    }

    SDL_Surface* instructionSurface = IMG_Load("resources/Instruction.png");
    if (instructionSurface) {
        instructionTexture = SDL_CreateTextureFromSurface(renderer, instructionSurface);
        SDL_FreeSurface(instructionSurface);
    } else {
        std::cout << "Warning: Could not load Instruction.png! " << IMG_GetError() << std::endl;
    }

    SDL_Surface* backSurface = IMG_Load("resources/Back_Button.png");
    if (backSurface) {
        backButtonTexture = SDL_CreateTextureFromSurface(renderer, backSurface);
        SDL_FreeSurface(backSurface);
    } else {
        std::cout << "Warning: Could not load Back_Button.png! " << IMG_GetError() << std::endl;
    }
}

void InstructionManager::render(SDL_Renderer* renderer) {
    if (backgroundTexture) {
        SDL_RenderCopy(renderer, backgroundTexture, nullptr, nullptr);
    } else {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        std::cout << "Warning: Instruction background not loaded, using black background!" << std::endl;
    }

    if (instructionTexture) {
        SDL_Rect instructionRect = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
        SDL_RenderCopy(renderer, instructionTexture, nullptr, &instructionRect);
    }

    if (backButtonTexture) {
        SDL_Rect backRect = {10, 10, 100, 100};
        SDL_RenderCopy(renderer, backButtonTexture, nullptr, &backRect);
    } else {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_Rect backRect = {10, 10, 100, 100};
        SDL_RenderFillRect(renderer, &backRect);
    }
}

void InstructionManager::handleClick(int x, int y, GameState& state) {
    SDL_Rect backRect = {10, 10, 100, 100};
    if (x >= backRect.x && x <= backRect.x + backRect.w && y >= backRect.y && y <= backRect.y + backRect.h) {
        state = MENU;
    }
}
