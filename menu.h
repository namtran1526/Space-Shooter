#ifndef MENU_H
#define MENU_H

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include "game.h"
#include "sound.h"

// Lớp MenuManager quản lý giao diện menu chính của trò chơi
class MenuManager {
private:
    SDL_Texture* backgroundTexture;
    SDL_Texture* playButtonTexture;
    SDL_Texture* instructionButtonTexture;
    SDL_Texture* musicButtonTexture;
    SDL_Texture* pauseButtonTexture;
    bool musicOn;
    SDL_Rect musicButtonSrcRect;
    SDL_Rect pauseButtonSrcRect;
    SoundManager* soundManager;
    void loadTextures(SDL_Renderer* renderer);
public:
    MenuManager(SDL_Renderer* renderer, SoundManager* sound);
    ~MenuManager();
    void render(SDL_Renderer* renderer, GameState state);
    void handleClick(int x, int y, GameState& state);
    bool isMusicOn() const { return musicOn; } // Kiểm tra xem nhạc có đang bật không
};

#endif
