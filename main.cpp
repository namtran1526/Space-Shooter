#include "game.h"
#include "score.h"
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <iostream>

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0 || !IMG_Init(IMG_INIT_PNG)) {
        std::cout << "Error: SDL/IMG_Init failed! " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Space Shooter",
                                        SDL_WINDOWPOS_CENTERED,
                                        SDL_WINDOWPOS_CENTERED,
                                        WINDOW_WIDTH, WINDOW_HEIGHT,
                                        0);
    if (!window) {
        std::cout << "Error: Could not create window! " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cout << "Error: Could not create renderer! " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    srand(time(nullptr));
    ScoreManager scoreManager(renderer);

    GameObject player = { {WINDOW_WIDTH/2 - PLAYER_WIDTH/2, WINDOW_HEIGHT - PLAYER_HEIGHT - 10, PLAYER_WIDTH, PLAYER_HEIGHT}, true };
    std::vector<GameObject> bullets;
    std::vector<GameObject> enemies;
    Uint32 lastEnemySpawn = 0;
    const Uint32 ENEMY_SPAWN_INTERVAL = 1000;
    Uint32 lastShotTime = 0;

    GameState state = MENU;
    SDL_Event event;
    const Uint8* keyboardState = SDL_GetKeyboardState(nullptr);

    while (true) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) goto cleanup;
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                scoreManager.handleClick(event.button.x, event.button.y, state);
            }
        }

        if (state == PLAYING) {
            if (keyboardState[SDL_SCANCODE_LEFT] && player.rect.x > 0) player.rect.x -= PLAYER_SPEED;
            if (keyboardState[SDL_SCANCODE_RIGHT] && player.rect.x < WINDOW_WIDTH - PLAYER_WIDTH) player.rect.x += PLAYER_SPEED;
            Uint32 currentTime = SDL_GetTicks();
            if (keyboardState[SDL_SCANCODE_SPACE] && currentTime - lastShotTime > BULLET_COOLDOWN) {
                bullets.push_back({{player.rect.x + PLAYER_WIDTH/2 - BULLET_WIDTH/2, player.rect.y - BULLET_HEIGHT, BULLET_WIDTH, BULLET_HEIGHT}, true});
                lastShotTime = currentTime;
            }

            if (currentTime - lastEnemySpawn > ENEMY_SPAWN_INTERVAL) {
                enemies.push_back({{rand() % (WINDOW_WIDTH - ENEMY_WIDTH), 0, ENEMY_WIDTH, ENEMY_HEIGHT}, true});
                lastEnemySpawn = currentTime;
            }

            for (auto& bullet : bullets) if (bullet.active) { bullet.rect.y -= BULLET_SPEED; if (bullet.rect.y < 0) bullet.active = false; }
            for (auto& enemy : enemies) if (enemy.active) { enemy.rect.y += ENEMY_SPEED; if (enemy.rect.y > WINDOW_HEIGHT) enemy.active = false; else if (player.active && SDL_HasIntersection(&player.rect, &enemy.rect)) { scoreManager.loseLife(); enemy.active = false; if (scoreManager.getLives() <= 0) { state = GAME_OVER; player.active = false; } } }
            for (size_t i = 0; i < bullets.size(); ++i) if (bullets[i].active) for (size_t j = 0; j < enemies.size(); ++j) if (enemies[j].active && SDL_HasIntersection(&bullets[i].rect, &enemies[j].rect)) { bullets[i].active = false; enemies[j].active = false; scoreManager.addScore(1); }

            bullets.erase(std::remove_if(bullets.begin(), bullets.end(), [](const GameObject& obj) { return !obj.active; }), bullets.end());
            enemies.erase(std::remove_if(enemies.begin(), enemies.end(), [](const GameObject& obj) { return !obj.active; }), enemies.end());
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        if (state == PLAYING && player.active) {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderFillRect(renderer, &player.rect);
            SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
            for (const auto& bullet : bullets) if (bullet.active) SDL_RenderFillRect(renderer, &bullet.rect);
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            for (const auto& enemy : enemies) if (enemy.active) SDL_RenderFillRect(renderer, &enemy.rect);
        }

        scoreManager.render(renderer, 10, 10, state);
        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

cleanup:
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
    return 0;
}
