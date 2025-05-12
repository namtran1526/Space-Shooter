#ifndef GAMEOVER_H
#define GAMEOVER_H

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include "game.h"
#include "score.h"
#include "player.h"
#include "bullet.h"
#include "enemy.h"
#include "sound.h"

class GameOverManager {
private:
    SDL_Texture* backgroundTexture;
    SDL_Texture* homeButtonTexture;
    SDL_Texture* resetButtonTexture;
    SoundManager* soundManager;
    void loadTextures(SDL_Renderer* renderer);

public:
    GameOverManager(SDL_Renderer* renderer, SoundManager* sound);
    ~GameOverManager();
    void render(SDL_Renderer* renderer);
    void handleClick(int x, int y, GameState& state, ScoreManager& scoreManager, Player& player, BulletManager& bullets, EnemyManager& enemies, Uint32& countdownStart, int& countdownStep, Uint32& lastEnemySpawn);
};

#endif
