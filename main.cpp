#include "game.h"
#include "player.h"
#include "bullet.h"
#include "enemy.h"
#include "explosion.h"
#include "menu.h"
#include "score.h"
#include "instruction.h"
#include "gameover.h"
#include "sound.h"
#include <cstdlib>
#include <ctime>
#include <iostream>

int main(int argc, char* argv[]) {
    // Tạo cửa sổ trò chơi
    SDL_Window* window = SDL_CreateWindow("Space Shooter",
                                        SDL_WINDOWPOS_CENTERED,
                                        SDL_WINDOWPOS_CENTERED,
                                        WINDOW_WIDTH, WINDOW_HEIGHT,
                                        0);
    // Tạo renderer để vẽ lên cửa sổ
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Khởi tạo random seed
    srand(time(nullptr));

    // Khởi tạo các thành phần trong trò chơi
    SoundManager soundManager;
    Player player(renderer);
    BulletManager bullets(renderer, &soundManager);
    EnemyManager enemies(renderer);
    ExplosionManager explosion(renderer, &soundManager);
    ScoreManager scoreManager;
    MenuManager menuManager(renderer, &soundManager);
    InstructionManager instructionManager(renderer, &soundManager);
    GameOverManager gameOverManager(renderer, &soundManager);

    // Tạo texture cho số mạng
    SDL_Texture* liveTexture = nullptr;
    SDL_Surface* liveSurface = IMG_Load("resources/Live.png");
    if (liveSurface) {
        liveTexture = SDL_CreateTextureFromSurface(renderer, liveSurface);
        SDL_FreeSurface(liveSurface);
    }

    // Tạo texture cho nền khi chơi
    SDL_Texture* playBGTexture = nullptr;
    SDL_Surface* playBGSurface = IMG_Load("resources/Play_BG.png");
    if (playBGSurface) {
        playBGTexture = SDL_CreateTextureFromSurface(renderer, playBGSurface);
        SDL_FreeSurface(playBGSurface);
    }

    // Biến điều khiển trạng thái trò chơi
    Uint32 countdownStart = 0;
    int countdownStep = 0;
    Uint32 lastEnemySpawn = 0;
    GameState state = MENU;

    // Bật nhạc nền cho menu
    soundManager.playMusicForState(MENU); // Phát nhạc Menu khi bắt đầu
    SDL_Event event;
    const Uint8* keyboardState = SDL_GetKeyboardState(nullptr);

    // Vòng lặp chính của trò chơi
    while (true) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                std::cout << "Quit event received, exiting game" << std::endl;
                break;
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
                        soundManager.playMusicForState(COUNTDOWN); // Phát nhạc Countdown
                        std::cout << "COUNTDOWN initialized" << std::endl;
                    } else if (state == INSTRUCTIONS) {
                        soundManager.playMusicForState(INSTRUCTIONS); // Phát nhạc Menu trong INSTRUCTIONS
                    }
                } else if (state == INSTRUCTIONS) {
                    instructionManager.handleClick(event.button.x, event.button.y, state);
                } else if (state == GAME_OVER) {
                    gameOverManager.handleClick(event.button.x, event.button.y, state, scoreManager, player, bullets, enemies, countdownStart, countdownStep, lastEnemySpawn);
                } else if (state == PLAYING || state == PAUSED) {
                    menuManager.handleClick(event.button.x, event.button.y, state);
                }
            }
        }

        // Xử lý trạng thái CONUTDOWN trước khi vào trạng thái PLAYING
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

        // Xử lý trạng thái PLAYING (đang chơi)
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
                            scoreManager.addScore(1); // Tăng điểm khi bắn trúng kẻ thù
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
                soundManager.playMusicForState(GAME_OVER); // Phát nhạc GameOver
                std::cout << "Explosion finished, transitioning to GAME_OVER" << std::endl;
            }
        }

        // Render giao diện MENU, INSTRUCTIONS và các trạng thái khác
        if (state == MENU) {
            menuManager.render(renderer, state);
        } else if (state == INSTRUCTIONS) {
            instructionManager.render(renderer);
        } else if (state == COUNTDOWN || state == PLAYING || state == PAUSED || state == EXPLODING || state == GAME_OVER) {
            if (playBGTexture) {
                SDL_RenderCopy(renderer, playBGTexture, nullptr, nullptr);
            }

            // Render các trạng thái đếm ngược
            if (state == COUNTDOWN) {
                const char* countdownTexts[] = {"Are you ready?", "3", "2", "1", "Start"};
                if (countdownStep < 5) {
                    TTF_Font* countdownFont = TTF_OpenFont("resources/VNI-Lithos.TTF", 48);
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
            } else if (state == GAME_OVER) {
                gameOverManager.render(renderer); // Render giao diện Game Over
            } else { // Render các đối tượng trong trạng thái chơi
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

        // Luôn render điểm số và số mạng trong trạng thái PLAYING
        scoreManager.render(renderer, 10, 10, state, liveTexture);
        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    // Giải phóng tài nguyên
    if (liveTexture) SDL_DestroyTexture(liveTexture);
    if (playBGTexture) SDL_DestroyTexture(playBGTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
    return 0;
}
