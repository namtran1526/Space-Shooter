#ifndef BULLET_H
#define BULLET_H

#include "game.h"
#include <vector>
#include "score.h"
#include "sound.h"

class BulletManager {
private:
    std::vector<GameObject> bullets;
    SDL_Texture* texture;
    Uint32 lastShotTime;
    SoundManager* soundManager;

public:
    BulletManager(SDL_Renderer* renderer, SoundManager* sound);
    ~BulletManager();
    void update(ScoreManager& scoreManager);
    void shoot(int x, int y, Uint32 currentTime, Uint32 cooldown);
    void render(SDL_Renderer* renderer);
    std::vector<GameObject>& getBullets() { return bullets; }
};

#endif
