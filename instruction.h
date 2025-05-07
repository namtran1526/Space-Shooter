#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include "game.h"

class InstructionManager {
private:
    SDL_Texture* instructionTexture;
    SDL_Texture* backButtonTexture;
    void loadTextures(SDL_Renderer* renderer);

public:
    InstructionManager(SDL_Renderer* renderer);
    ~InstructionManager();
    void render(SDL_Renderer* renderer);
    void handleClick(int x, int y, GameState& state);
};

#endif
