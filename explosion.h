#ifndef EXPLOSION_H
#define EXPLOSION_H

#include "game.h"
#include "sound.h"

class ExplosionManager {
private:
    Explosion explosion;
    SDL_Texture* texture;
    SoundManager* soundManager;

public:
    ExplosionManager(SDL_Renderer* renderer, SoundManager* sound);
    ~ExplosionManager();
    void update();
    void trigger(int x, int y);
    void render(SDL_Renderer* renderer);
    bool isActive() const { return explosion.active; }
};

#endif
