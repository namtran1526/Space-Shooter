#include "instruction.h"

// Constructor khởi tạo Instruction
InstructionManager::InstructionManager(SDL_Renderer* renderer, SoundManager* sound) : backgroundTexture(nullptr), instructionTexture(nullptr), backButtonTexture(nullptr), soundManager(sound) {
    loadTextures(renderer);
}

// Destructor giải phóng Instruction
InstructionManager::~InstructionManager() {
    if (backgroundTexture) SDL_DestroyTexture(backgroundTexture);
    if (instructionTexture) SDL_DestroyTexture(instructionTexture);
    if (backButtonTexture) SDL_DestroyTexture(backButtonTexture);
}

// Hàm load các texture từ file
void InstructionManager::loadTextures(SDL_Renderer* renderer) {
    SDL_Surface* bgSurface = IMG_Load("resources/Play_BG.png");
    if (bgSurface) {
        backgroundTexture = SDL_CreateTextureFromSurface(renderer, bgSurface);
        SDL_FreeSurface(bgSurface);
    }
    SDL_Surface* instructionSurface = IMG_Load("resources/Instruction.png");
    if (instructionSurface) {
        instructionTexture = SDL_CreateTextureFromSurface(renderer, instructionSurface);
        SDL_FreeSurface(instructionSurface);
    }
    SDL_Surface* backSurface = IMG_Load("resources/Back_Button.png");
    if (backSurface) {
        backButtonTexture = SDL_CreateTextureFromSurface(renderer, backSurface);
        SDL_FreeSurface(backSurface);
    }
}

// Render giao diện hướng dẫn
void InstructionManager::render(SDL_Renderer* renderer) {
    if (backgroundTexture) {
        SDL_RenderCopy(renderer, backgroundTexture, nullptr, nullptr);
    }

    if (instructionTexture) {
        SDL_Rect instructionRect = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
        SDL_RenderCopy(renderer, instructionTexture, nullptr, &instructionRect);
    }

    if (backButtonTexture) {
        SDL_Rect backRect = {10, 10, 100, 100};
        SDL_RenderCopy(renderer, backButtonTexture, nullptr, &backRect);
    }
}

// Hàm xử lý sự kiện click chuột
void InstructionManager::handleClick(int x, int y, GameState& state) {
    SDL_Rect backRect = {10, 10, 100, 100};
    if (x >= backRect.x && x <= backRect.x + backRect.w && y >= backRect.y && y <= backRect.y + backRect.h) {
        state = MENU;
        soundManager->playMusicForState(MENU); // Phát nhạc Menu
    }
}
