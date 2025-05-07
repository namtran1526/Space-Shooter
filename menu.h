#ifndef MENU_H
#define MENU_H

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include "game.h"

class MenuManager {
private:
    SDL_Texture* backgroundTexture;
    SDL_Texture* playButtonTexture;
    SDL_Texture* instructionButtonTexture;
    SDL_Texture* musicButtonTexture;
    bool musicOn;
    SDL_Rect musicButtonSrcRect;
    void loadTextures(SDL_Renderer* renderer);

public:
    MenuManager(SDL_Renderer* renderer);
    ~MenuManager();
    void render(SDL_Renderer* renderer);
    void handleClick(int x, int y, GameState& state);
    bool isMusicOn() const { return musicOn; }
};

#endif
