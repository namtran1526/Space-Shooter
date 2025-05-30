#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include "game.h"
#include "sound.h"

// Lớp InstructionManager quản lý giao diện hướng dẫn chơi game
class InstructionManager {
private:
    SDL_Texture* backgroundTexture;
    SDL_Texture* instructionTexture;
    SDL_Texture* backButtonTexture;
    SoundManager* soundManager;
    void loadTextures(SDL_Renderer* renderer);

public:
    InstructionManager(SDL_Renderer* renderer, SoundManager* sound);
    ~InstructionManager();
    void render(SDL_Renderer* renderer);
    void handleClick(int x, int y, GameState& state);
};

#endif
