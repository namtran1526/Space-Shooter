#include "instruction.h"

InstructionManager::InstructionManager(SDL_Renderer* renderer) : instructionTexture(nullptr), backButtonTexture(nullptr) {
    loadTextures(renderer);
}

InstructionManager::~InstructionManager() {
    if (instructionTexture) SDL_DestroyTexture(instructionTexture);
    if (backButtonTexture) SDL_DestroyTexture(backButtonTexture);
}

void InstructionManager::loadTextures(SDL_Renderer* renderer) {
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
    SDL_Surface* playBGSurface = IMG_Load("resources/Play_BG.png");
    SDL_Texture* playBGTexture = nullptr;
    if (playBGSurface) {
        playBGTexture = SDL_CreateTextureFromSurface(renderer, playBGSurface);
        SDL_FreeSurface(playBGSurface);
        if (playBGTexture) {
            SDL_RenderCopy(renderer, playBGTexture, nullptr, nullptr);
            SDL_DestroyTexture(playBGTexture);
        } else {
            std::cout << "Warning: Could not create Play_BG texture in Instruction!" << std::endl;
        }
    } else {
        std::cout << "Warning: Could not load Play_BG.png in Instruction! " << IMG_GetError() << std::endl;
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Fallback to black background
        SDL_RenderClear(renderer);
    }

    if (instructionTexture) {
        SDL_Rect instructionRect = {0, 0, 800, 600};
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
