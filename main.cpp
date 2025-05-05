#include "game.h"
#include "score.h"
#include <cstdlib>
#include <ctime>
#include <algorithm>

int main(int argc, char* argv[]) {
    // Initialize SDL
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Space Shooter",
                                        SDL_WINDOWPOS_CENTERED,
                                        SDL_WINDOWPOS_CENTERED,
                                        WINDOW_WIDTH, WINDOW_HEIGHT,
                                        0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Initialize random seed
    srand(time(nullptr));

    // Initialize score manager
    ScoreManager scoreManager;

    // Player
    GameObject player;
    player.rect = {WINDOW_WIDTH/2 - PLAYER_WIDTH/2, WINDOW_HEIGHT - PLAYER_HEIGHT - 10,
                   PLAYER_WIDTH, PLAYER_HEIGHT};
    player.active = true;

    // Bullets
    std::vector<GameObject> bullets;

    // Enemies
    std::vector<GameObject> enemies;
    Uint32 lastEnemySpawn = 0;
    const Uint32 ENEMY_SPAWN_INTERVAL = 1000; // Spawn every 1 second
    Uint32 lastShotTime = 0; // Track last shot time

    int collisionCount = 0;
    bool running = true;
    SDL_Event event;
    const Uint8* keyboardState = SDL_GetKeyboardState(nullptr);

    while (running) {
        // Handle events
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        // Handle input
        if (keyboardState[SDL_SCANCODE_LEFT] && player.rect.x > 0) {
            player.rect.x -= PLAYER_SPEED;
        }
        if (keyboardState[SDL_SCANCODE_RIGHT] && player.rect.x < WINDOW_WIDTH - PLAYER_WIDTH) {
            player.rect.x += PLAYER_SPEED;
        }
        Uint32 currentTime = SDL_GetTicks();
        if (keyboardState[SDL_SCANCODE_SPACE] && currentTime - lastShotTime > BULLET_COOLDOWN) {
            // Shoot bullet with cooldown
            GameObject bullet;
            bullet.rect = {player.rect.x + PLAYER_WIDTH/2 - BULLET_WIDTH/2,
                          player.rect.y - BULLET_HEIGHT,
                          BULLET_WIDTH, BULLET_HEIGHT};
            bullet.active = true;
            bullets.push_back(bullet);
            lastShotTime = currentTime;
        }

        // Spawn enemies
        if (currentTime - lastEnemySpawn > ENEMY_SPAWN_INTERVAL) {
            GameObject enemy;
            enemy.rect = {rand() % (WINDOW_WIDTH - ENEMY_WIDTH), 0,
                         ENEMY_WIDTH, ENEMY_HEIGHT};
            enemy.active = true;
            enemies.push_back(enemy);
            lastEnemySpawn = currentTime;
        }

        // Update bullets
        for (auto& bullet : bullets) {
            if (bullet.active) {
                bullet.rect.y -= BULLET_SPEED;
                if (bullet.rect.y < 0) {
                    bullet.active = false;
                }
            }
        }

        // Update enemies
        for (auto& enemy : enemies) {
            if (enemy.active) {
                enemy.rect.y += ENEMY_SPEED;
                if (enemy.rect.y > WINDOW_HEIGHT) {
                    enemy.active = false;
                }
                // Check player collision
                if (SDL_HasIntersection(&player.rect, &enemy.rect) && enemy.active) {
                    scoreManager.loseLife();
                    enemy.active = false;
                    if (scoreManager.getLives() <= 0) {
                        running = false; // Game over when no lives left
                    }
                }
            }
        }

        // Check bullet-enemy collisions
        for (size_t i = 0; i < bullets.size(); ++i) {
            if (!bullets[i].active) continue;
            for (size_t j = 0; j < enemies.size(); ++j) {
                if (!enemies[j].active) continue;
                if (SDL_HasIntersection(&bullets[i].rect, &enemies[j].rect)) {
                    bullets[i].active = false;
                    enemies[j].active = false;
                    scoreManager.addScore(1); // Add 1 point for each enemy destroyed
                }
            }
        }

        // Remove inactive objects
        bullets.erase(std::remove_if(bullets.begin(), bullets.end(),
            [](const GameObject& obj) { return !obj.active; }), bullets.end());
        enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
            [](const GameObject& obj) { return !obj.active; }), enemies.end());

        // Render
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Draw player
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &player.rect);

        // Draw bullets
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
        for (const auto& bullet : bullets) {
            if (bullet.active) {
                SDL_RenderFillRect(renderer, &bullet.rect);
            }
        }

        // Draw enemies
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        for (const auto& enemy : enemies) {
            if (enemy.active) {
                SDL_RenderFillRect(renderer, &enemy.rect);
            }
        }

        // Render score and lives
        scoreManager.render(renderer, 10, 10);

        SDL_RenderPresent(renderer);
        SDL_Delay(16); // ~60 FPS
    }

    // Cleanup
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
