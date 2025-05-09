#ifndef ENEMY_H
#define ENEMY_H

#include "game.h"
#include <vector>
#include "score.h"

class EnemyManager {
private:
    std::vector<GameObject> enemies;
    SDL_Texture* texture;
    Uint32 lastSpawnTime;
    const Uint32 SPAWN_INTERVAL = 1000;

public:
    EnemyManager(SDL_Renderer* renderer);
    ~EnemyManager();
    void update(ScoreManager& scoreManager, const GameObject& player); // Sửa thành const GameObject&
    void spawn(Uint32 currentTime);
    void render(SDL_Renderer* renderer);
    std::vector<GameObject>& getEnemies() { return enemies; }
};

#endif
