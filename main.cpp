#include "game.h"
#include "player.h"
#include "bullet.h"
#include "enemy.h"
#include "explosion.h"
#include "menu.h"
#include "score.h"
#include "instruction.h"
#include <cstdlib>
#include <ctime>
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
    Player player(renderer);
    BulletManager bullets(renderer);
    EnemyManager enemies(renderer);
    ExplosionManager explosion(renderer);
    ScoreManager scoreManager;
    MenuManager menuManager(renderer);
    InstructionManager instructionManager(renderer);

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

    Uint32 countdownStart = 0;
    int countdownStep = 0;
    Uint32 lastEnemySpawn = 0;

    GameState state = MENU;
    SDL_Event event;
    const Uint8* keyboardState = SDL_GetKeyboardState(nullptr);

    while (true) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                std::cout << "Quit event received, exiting game" << std::endl;
                goto cleanup;
            }
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                if (state == MENU) {
                    menuManager.handleClick(event.button.x, event.button.y, state);
                    if (state == COUNTDOWN) {
                        scoreManager.reset();
                        player.resetPosition();
                        bullets.getBullets().clear();
                        enemies.getEnemies().clear();
                        countdownStart = SDL_GetTicks();
                        countdownStep = 0;
                        lastEnemySpawn = 0;
                        std::cout << "COUNTDOWN initialized" << std::endl;
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
                            player.resetPosition();
                            bullets.getBullets().clear();
                            enemies.getEnemies().clear();
                            countdownStart = SDL_GetTicks();
                            countdownStep = 0;
                            lastEnemySpawn = 0;
                            std::cout << "Clicked Play Again, transitioning to COUNTDOWN" << std::endl;
                        }
                    }
                } else if (state == PLAYING || state == PAUSED) {
                    menuManager.handleClick(event.button.x, event.button.y, state);
                }
            }
        }

        if (state == COUNTDOWN) {
            Uint32 currentTime = SDL_GetTicks();
            int elapsedSteps = (currentTime - countdownStart) / COUNTDOWN_DELAY;
            countdownStep = elapsedSteps;
            if (countdownStep >= 5) {
                state = PLAYING;
                countdownStep = 0;
                std::cout << "State changed to PLAYING" << std::endl;
            }
        }

        if (state == PLAYING) {
            player.update(keyboardState);
            Uint32 currentTime = SDL_GetTicks();
            if (keyboardState[SDL_SCANCODE_SPACE]) {
                bullets.shoot(player.getObject().rect.x + PLAYER_WIDTH/2, player.getObject().rect.y, currentTime, BULLET_COOLDOWN);
            }
            enemies.spawn(currentTime);
            bullets.update(scoreManager);
            enemies.update(scoreManager, player.getObject());

            // Kiểm tra va chạm giữa đạn và kẻ thù
            auto& bulletList = bullets.getBullets();
            auto& enemyList = enemies.getEnemies();
            for (size_t i = 0; i < bulletList.size(); ++i) {
                if (bulletList[i].active) {
                    for (size_t j = 0; j < enemyList.size(); ++j) {
                        if (enemyList[j].active && SDL_HasIntersection(&bulletList[i].rect, &enemyList[j].rect)) {
                            bulletList[i].active = false;
                            enemyList[j].active = false;
                            scoreManager.addScore(1);
                            std::cout << "Enemy hit by bullet, score increased to: " << scoreManager.getScore() << std::endl;
                        }
                    }
                }
            }

            // Kiểm tra xem player có hết mạng không
            if (scoreManager.getLives() <= 0 && player.getObject().active) {
                explosion.trigger(player.getObject().rect.x, player.getObject().rect.y);
                player.getObject().active = false; // Vô hiệu hóa player
                state = EXPLODING;
                std::cout << "Player died, transitioning to EXPLODING" << std::endl;
            }
        }

        if (state == EXPLODING) {
            explosion.update();
            if (!explosion.isActive()) {
                state = GAME_OVER;
                std::cout << "Explosion finished, transitioning to GAME_OVER" << std::endl;
            }
        }

        // Render
        if (state == MENU) {
            menuManager.render(renderer, state);
        } else if (state == INSTRUCTIONS) {
            instructionManager.render(renderer);
        } else if (state == COUNTDOWN || state == PLAYING || state == PAUSED || state == EXPLODING || state == GAME_OVER) {
            if (playBGTexture) {
                SDL_RenderCopy(renderer, playBGTexture, nullptr, nullptr);
            } else {
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderClear(renderer);
                std::cout << "Warning: Play_BG not loaded, using black background!" << std::endl;
            }

            if (state == COUNTDOWN) {
                const char* countdownTexts[] = {"Are you ready?", "3", "2", "1", "Start"};
                if (countdownStep < 5) {
                    TTF_Font* countdownFont = TTF_OpenFont("C:\\Windows\\Fonts\\arialbd.ttf", 48);
                    if (!countdownFont) {
                        std::cout << "Warning: Could not load bold font at 'C:\\Windows\\Fonts\\arialbd.ttf'! SDL_ttf Error: " << TTF_GetError() << std::endl;
                        std::cout << "Falling back to default font from ScoreManager." << std::endl;
                        countdownFont = scoreManager.getFont();
                        if (countdownFont) TTF_SetFontSize(countdownFont, 48);
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
                        }
                        if (countdownFont != scoreManager.getFont()) TTF_CloseFont(countdownFont);
                    }
                }
            } else {
                player.render(renderer);
                bullets.render(renderer);
                enemies.render(renderer);
                if (state == EXPLODING) {
                    explosion.render(renderer);
                }
                if (state == PLAYING || state == PAUSED) {
                    menuManager.render(renderer, state);
                }
            }
        }

        scoreManager.render(renderer, 10, 10, state, liveTexture);
        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

cleanup:
    if (liveTexture) SDL_DestroyTexture(liveTexture);
    if (playBGTexture) SDL_DestroyTexture(playBGTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
    return 0;
}
