#include "game.h"
#include "score.h"
#include "menu.h"
#include "instruction.h"
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
    ScoreManager scoreManager;
    MenuManager menuManager(renderer);
    InstructionManager instructionManager(renderer);

    // Load textures with fallback
    SDL_Texture* playerTexture = nullptr;
    SDL_Surface* playerSurface = IMG_Load("resources/Player.png");
    if (playerSurface) {
        playerTexture = SDL_CreateTextureFromSurface(renderer, playerSurface);
        SDL_FreeSurface(playerSurface);
    } else {
        std::cout << "Warning: Could not load Player.png! Using fallback. " << IMG_GetError() << std::endl;
    }

    SDL_Texture* enemyTexture = nullptr;
    SDL_Surface* enemySurface = IMG_Load("resources/Enemy.png");
    if (enemySurface) {
        enemyTexture = SDL_CreateTextureFromSurface(renderer, enemySurface);
        SDL_FreeSurface(enemySurface);
    } else {
        std::cout << "Warning: Could not load Enemy.png! Using fallback. " << IMG_GetError() << std::endl;
    }

    SDL_Texture* bulletTexture = nullptr;
    SDL_Surface* bulletSurface = IMG_Load("resources/Bullet.png");
    if (bulletSurface) {
        bulletTexture = SDL_CreateTextureFromSurface(renderer, bulletSurface);
        SDL_FreeSurface(bulletSurface);
    } else {
        std::cout << "Warning: Could not load Bullet.png! Using fallback. " << IMG_GetError() << std::endl;
    }

    SDL_Texture* liveTexture = nullptr;
    SDL_Surface* liveSurface = IMG_Load("resources/Live.png");
    if (liveSurface) {
        liveTexture = SDL_CreateTextureFromSurface(renderer, liveSurface);
        SDL_FreeSurface(liveSurface);
    } else {
        std::cout << "Warning: Could not load Live.png! Using fallback. " << IMG_GetError() << std::endl;
    }

    SDL_Texture* playBGTexture = nullptr;
    SDL_Surface* playBGSurface = IMG_Load("resources/Play_BG.png");
    if (playBGSurface) {
        playBGTexture = SDL_CreateTextureFromSurface(renderer, playBGSurface);
        SDL_FreeSurface(playBGSurface);
    } else {
        std::cout << "Warning: Could not load Play_BG.png! Using fallback. " << IMG_GetError() << std::endl;
    }

    GameObject player = { {WINDOW_WIDTH/2 - PLAYER_WIDTH/2, WINDOW_HEIGHT - PLAYER_HEIGHT - 10, PLAYER_WIDTH, PLAYER_HEIGHT}, true, playerTexture };
    std::vector<GameObject> bullets;
    std::vector<GameObject> enemies;
    Uint32 lastEnemySpawn = 0;
    const Uint32 ENEMY_SPAWN_INTERVAL = 1000;
    Uint32 lastShotTime = 0;
    Uint32 countdownStart = 0;
    int countdownStep = 0;

    GameState state = MENU;
    SDL_Event event;
    const Uint8* keyboardState = SDL_GetKeyboardState(nullptr);

    while (true) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) goto cleanup;
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                if (state == MENU) {
                    menuManager.handleClick(event.button.x, event.button.y, state);
                    if (state == COUNTDOWN) {
                        scoreManager.reset();
                        player = { {WINDOW_WIDTH/2 - PLAYER_WIDTH/2, WINDOW_HEIGHT - PLAYER_HEIGHT - 10, PLAYER_WIDTH, PLAYER_HEIGHT}, true, playerTexture };
                        bullets.clear();
                        enemies.clear();
                        lastEnemySpawn = 0;
                        lastShotTime = 0;
                        countdownStart = SDL_GetTicks();
                        countdownStep = 0;
                    }
                } else if (state == INSTRUCTIONS) {
                    instructionManager.handleClick(event.button.x, event.button.y, state);
                } else if (state == GAME_OVER) {
                    SDL_Surface* playAgainSurface = TTF_RenderText_Solid(scoreManager.getFont(), "Play Again", scoreManager.getTextColor());
                    if (playAgainSurface) {
                        SDL_Rect playAgainRect = {(WINDOW_WIDTH - playAgainSurface->w) / 2, (WINDOW_HEIGHT - playAgainSurface->h) / 2 + 50, playAgainSurface->w, playAgainSurface->h};
                        SDL_Rect buttonRect = {playAgainRect.x - 10, playAgainRect.y - 10, playAgainRect.w + 20, playAgainRect.h + 20};
                        SDL_FreeSurface(playAgainSurface);
                        if (event.button.x >= buttonRect.x && event.button.x <= buttonRect.x + buttonRect.w &&
                            event.button.y >= buttonRect.y && event.button.y <= buttonRect.y + buttonRect.h) {
                            state = COUNTDOWN;
                            scoreManager.reset();
                            player = { {WINDOW_WIDTH/2 - PLAYER_WIDTH/2, WINDOW_HEIGHT - PLAYER_HEIGHT - 10, PLAYER_WIDTH, PLAYER_HEIGHT}, true, playerTexture };
                            player.active = true; // Ensure player is active when restarting
                            bullets.clear();
                            enemies.clear();
                            lastEnemySpawn = 0;
                            lastShotTime = 0;
                            countdownStart = SDL_GetTicks();
                            countdownStep = 0;
                        }
                    }
                }
            }
        }

        if (state == COUNTDOWN) {
            Uint32 currentTime = SDL_GetTicks();
            int elapsedSteps = (currentTime - countdownStart) / COUNTDOWN_DELAY;
            countdownStep = elapsedSteps;
            if (countdownStep >= 5) { // Sau "Are you ready?", "3", "2", "1", "Start"
                state = PLAYING;
                countdownStep = 0;
            }
        }

        if (state == PLAYING) {
            if (keyboardState[SDL_SCANCODE_LEFT] && player.rect.x > 0) player.rect.x -= PLAYER_SPEED;
            if (keyboardState[SDL_SCANCODE_RIGHT] && player.rect.x < WINDOW_WIDTH - PLAYER_WIDTH) player.rect.x += PLAYER_SPEED;
            Uint32 currentTime = SDL_GetTicks();
            if (keyboardState[SDL_SCANCODE_SPACE] && currentTime - lastShotTime > BULLET_COOLDOWN) {
                bullets.push_back({{player.rect.x + PLAYER_WIDTH/2 - BULLET_WIDTH/2, player.rect.y - BULLET_HEIGHT, BULLET_WIDTH, BULLET_HEIGHT}, true, bulletTexture});
                lastShotTime = currentTime;
            }

            if (currentTime - lastEnemySpawn > ENEMY_SPAWN_INTERVAL) {
                enemies.push_back({{rand() % (WINDOW_WIDTH - ENEMY_WIDTH), 0, ENEMY_WIDTH, ENEMY_HEIGHT}, true, enemyTexture});
                lastEnemySpawn = currentTime;
            }

            for (auto& bullet : bullets) if (bullet.active) { bullet.rect.y -= BULLET_SPEED; if (bullet.rect.y < 0) bullet.active = false; }
            for (auto& enemy : enemies) if (enemy.active) {
                enemy.rect.y += ENEMY_SPEED;
                if (enemy.rect.y > WINDOW_HEIGHT) enemy.active = false;
                else if (player.active && SDL_HasIntersection(&player.rect, &enemy.rect)) {
                    scoreManager.loseLife();
                    enemy.active = false;
                    if (scoreManager.getLives() <= 0) {
                        state = GAME_OVER;
                        player.active = false;
                    }
                }
            }
            for (size_t i = 0; i < bullets.size(); ++i) if (bullets[i].active) for (size_t j = 0; j < enemies.size(); ++j) if (enemies[j].active && SDL_HasIntersection(&bullets[i].rect, &enemies[j].rect)) {
                bullets[i].active = false;
                enemies[j].active = false;
                scoreManager.addScore(1);
            }

            bullets.erase(std::remove_if(bullets.begin(), bullets.end(), [](const GameObject& obj) { return !obj.active; }), bullets.end());
            enemies.erase(std::remove_if(enemies.begin(), enemies.end(), [](const GameObject& obj) { return !obj.active; }), enemies.end());
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        if (state == MENU) {
            menuManager.render(renderer);
        } else if (state == INSTRUCTIONS) {
            instructionManager.render(renderer);
        } else if (state == COUNTDOWN || state == PLAYING || state == GAME_OVER) {
            if (playBGTexture) {
                SDL_RenderCopy(renderer, playBGTexture, nullptr, nullptr);
            } else {
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Fallback to black background
                SDL_RenderClear(renderer);
            }
        }

        if (state == COUNTDOWN) {
            const char* countdownTexts[] = {"Are you ready?", "3", "2", "1", "Start"};
            if (countdownStep < 5) {
                TTF_Font* countdownFont = TTF_OpenFont("C:\\Windows\\Fonts\\arialbd.ttf", 48);
                if (!countdownFont) {
                    std::cout << "Warning: Could not load bold font! Using default font. " << TTF_GetError() << std::endl;
                    countdownFont = scoreManager.getFont();
                    if (countdownFont) {
                        TTF_SetFontSize(countdownFont, 48); // Ensure size is 48 even with default font
                    }
                }
                if (countdownFont) {
                    SDL_Surface* countdownSurface = TTF_RenderText_Solid(countdownFont, countdownTexts[countdownStep], scoreManager.getTextColor());
                    if (countdownSurface) {
                        SDL_Texture* countdownTexture = SDL_CreateTextureFromSurface(renderer, countdownSurface);
                        if (countdownTexture) {
                            SDL_Rect countdownRect = {(WINDOW_WIDTH - countdownSurface->w) / 2, (WINDOW_HEIGHT - countdownSurface->h) / 2, countdownSurface->w, countdownSurface->h};
                            SDL_RenderCopy(renderer, countdownTexture, nullptr, &countdownRect);
                            SDL_DestroyTexture(countdownTexture);
                        }
                        SDL_FreeSurface(countdownSurface);
                    } else {
                        std::cout << "Error: Could not render countdown text! " << TTF_GetError() << std::endl;
                    }
                    if (countdownFont != scoreManager.getFont()) TTF_CloseFont(countdownFont);
                }
            }
        } else if (state == PLAYING && player.active) {
            if (player.texture) {
                SDL_RenderCopy(renderer, player.texture, nullptr, &player.rect);
            } else {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                SDL_RenderFillRect(renderer, &player.rect);
            }
            for (const auto& bullet : bullets) {
                if (bullet.active) {
                    if (bullet.texture) {
                        SDL_RenderCopy(renderer, bullet.texture, nullptr, &bullet.rect);
                    } else {
                        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
                        SDL_RenderFillRect(renderer, &bullet.rect);
                    }
                }
            }
            for (const auto& enemy : enemies) {
                if (enemy.active) {
                    if (enemy.texture) {
                        SDL_RenderCopy(renderer, enemy.texture, nullptr, &enemy.rect);
                    } else {
                        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
                        SDL_RenderFillRect(renderer, &enemy.rect);
                    }
                }
            }
        }

        scoreManager.render(renderer, 10, 10, state, liveTexture);
        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

cleanup:
    if (playerTexture) SDL_DestroyTexture(playerTexture);
    if (enemyTexture) SDL_DestroyTexture(enemyTexture);
    if (bulletTexture) SDL_DestroyTexture(bulletTexture);
    if (liveTexture) SDL_DestroyTexture(liveTexture);
    if (playBGTexture) SDL_DestroyTexture(playBGTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
    return 0;
}
