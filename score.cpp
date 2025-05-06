#include "score.h"
#include "game.h"
#include <fstream>
#include <sstream>
#include <iostream>

ScoreManager::ScoreManager(SDL_Renderer* renderer) : score(0), highScore(0), lives(3), font(nullptr), backgroundTexture(nullptr), playButtonTexture(nullptr), instructionButtonTexture(nullptr), musicButtonTexture(nullptr), musicOffButtonTexture(nullptr), musicOn(true) {
    if (TTF_Init() < 0) {
        std::cout << "Error: TTF_Init failed! " << TTF_GetError() << std::endl;
        return;
    }
    font = TTF_OpenFont("C:\\Windows\\Fonts\\arial.ttf", 24);
    if (!font) {
        std::cout << "Error: Could not load font! " << TTF_GetError() << std::endl;
    }
    textColor = {255, 255, 255, 255};
    loadHighScore();
    loadTextures(renderer);
}

ScoreManager::~ScoreManager() {
    saveHighScore();
    if (font) TTF_CloseFont(font);
    if (backgroundTexture) SDL_DestroyTexture(backgroundTexture);
    if (playButtonTexture) SDL_DestroyTexture(playButtonTexture);
    if (instructionButtonTexture) SDL_DestroyTexture(instructionButtonTexture);
    if (musicButtonTexture) SDL_DestroyTexture(musicButtonTexture);
    if (musicOffButtonTexture) SDL_DestroyTexture(musicOffButtonTexture);
    TTF_Quit();
}

void ScoreManager::loadHighScore() {
    std::ifstream file(highScoreFile);
    if (file.is_open()) {
        file >> highScore;
        file.close();
    }
}

void ScoreManager::saveHighScore() {
    if (score > highScore) {
        highScore = score;
        std::ofstream file(highScoreFile);
        if (file.is_open()) {
            file << highScore;
            file.close();
        }
    }
}

void ScoreManager::addScore(int points) {
    score += points;
}

int ScoreManager::getScore() const {
    return score;
}

int ScoreManager::getHighScore() const {
    return highScore;
}

int ScoreManager::getLives() const {
    return lives;
}

void ScoreManager::loseLife() {
    if (lives > 0) lives--;
}

void ScoreManager::reset() {
    score = 0;
    lives = 3;
}

void ScoreManager::loadTextures(SDL_Renderer* renderer) {
    SDL_Surface* backgroundSurface = IMG_Load("background.png"); // Thay bằng đường dẫn ảnh nền của bạn
    if (backgroundSurface) {
        backgroundTexture = SDL_CreateTextureFromSurface(renderer, backgroundSurface);
        SDL_FreeSurface(backgroundSurface);
    }

    SDL_Surface* playSurface = IMG_Load("play_button.png"); // Thay bằng đường dẫn ảnh nút Play 200x200
    if (playSurface) {
        playButtonTexture = SDL_CreateTextureFromSurface(renderer, playSurface);
        SDL_FreeSurface(playSurface);
    }

    SDL_Surface* instructionSurface = IMG_Load("instruction_button.png"); // Thay bằng đường dẫn ảnh nút Instruction 200x200
    if (instructionSurface) {
        instructionButtonTexture = SDL_CreateTextureFromSurface(renderer, instructionSurface);
        SDL_FreeSurface(instructionSurface);
    }

    SDL_Surface* musicSurface = IMG_Load("music_button.png"); // Thay bằng đường dẫn ảnh nút Music 400x200
    if (musicSurface) {
        musicButtonTexture = SDL_CreateTextureFromSurface(renderer, musicSurface);
        SDL_FreeSurface(musicSurface);
    }

    SDL_Surface* musicOffSurface = IMG_Load("music_off_button.png"); // Thay bằng đường dẫn ảnh Music Off 400x200
    if (musicOffSurface) {
        musicOffButtonTexture = SDL_CreateTextureFromSurface(renderer, musicOffSurface);
        SDL_FreeSurface(musicOffSurface);
    }
}

void ScoreManager::render(SDL_Renderer* renderer, int x, int y, GameState state) {
    if (!font) {
        std::cout << "Error: Font not loaded, cannot render text!" << std::endl;
        return;
    }

    if (state == MENU) {
        if (backgroundTexture) {
            SDL_RenderCopy(renderer, backgroundTexture, nullptr, nullptr);
        }

        if (playButtonTexture) {
            SDL_Rect playRect = {(WINDOW_WIDTH - 200) / 2 - 100, (WINDOW_HEIGHT - 200) / 2, 200, 200};
            SDL_RenderCopy(renderer, playButtonTexture, nullptr, &playRect);
        }

        if (instructionButtonTexture) {
            SDL_Rect instructionRect = {(WINDOW_WIDTH - 200) / 2 + 100, (WINDOW_HEIGHT - 200) / 2, 200, 200};
            SDL_RenderCopy(renderer, instructionButtonTexture, nullptr, &instructionRect);
        }

        if (musicButtonTexture && musicOffButtonTexture) {
            SDL_Rect musicRect = {(WINDOW_WIDTH - 400) / 2, (WINDOW_HEIGHT - 200) / 2 + 250, 400, 200};
            SDL_RenderCopy(renderer, musicOn ? musicButtonTexture : musicOffButtonTexture, nullptr, &musicRect);
        }
    } else if (state == PLAYING) {
        std::string scoreText = "Score: " + std::to_string(score);
        std::string highScoreText = "High Score: " + std::to_string(highScore);

        SDL_Surface* scoreSurface = TTF_RenderText_Solid(font, scoreText.c_str(), textColor);
        SDL_Surface* highScoreSurface = TTF_RenderText_Solid(font, highScoreText.c_str(), textColor);
        if (scoreSurface && highScoreSurface) {
            SDL_Texture* scoreTexture = SDL_CreateTextureFromSurface(renderer, scoreSurface);
            SDL_Texture* highScoreTexture = SDL_CreateTextureFromSurface(renderer, highScoreSurface);
            if (scoreTexture && highScoreTexture) {
                SDL_Rect scoreRect = {x, y, scoreSurface->w, scoreSurface->h};
                SDL_Rect highScoreRect = {x, y + 30, highScoreSurface->w, highScoreSurface->h};
                SDL_RenderCopy(renderer, scoreTexture, nullptr, &scoreRect);
                SDL_RenderCopy(renderer, highScoreTexture, nullptr, &highScoreRect);
            }
            if (scoreTexture) SDL_DestroyTexture(scoreTexture);
            if (highScoreTexture) SDL_DestroyTexture(highScoreTexture);
        }
        if (scoreSurface) SDL_FreeSurface(scoreSurface);
        if (highScoreSurface) SDL_FreeSurface(highScoreSurface);

        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        const int heartSize = 20;
        const int heartSpacing = 5;
        for (int i = 0; i < lives && i < 3; ++i) {
            SDL_Rect heartRect = {x + i * (heartSize + heartSpacing), y + 60, heartSize, heartSize};
            SDL_RenderFillRect(renderer, &heartRect);
        }
    } else if (state == GAME_OVER) {
        std::string gameOverText = "Game Over";
        std::string playAgainText = "Play Again";
        SDL_Surface* gameOverSurface = TTF_RenderText_Solid(font, gameOverText.c_str(), textColor);
        SDL_Surface* playAgainSurface = TTF_RenderText_Solid(font, playAgainText.c_str(), textColor);
        if (gameOverSurface && playAgainSurface) {
            SDL_Texture* gameOverTexture = SDL_CreateTextureFromSurface(renderer, gameOverSurface);
            SDL_Texture* playAgainTexture = SDL_CreateTextureFromSurface(renderer, playAgainSurface);
            if (gameOverTexture) {
                SDL_Rect gameOverRect = {(WINDOW_WIDTH - gameOverSurface->w) / 2, (WINDOW_HEIGHT - gameOverSurface->h) / 2 - 50, gameOverSurface->w, gameOverSurface->h};
                SDL_RenderCopy(renderer, gameOverTexture, nullptr, &gameOverRect);
                SDL_DestroyTexture(gameOverTexture);
            }
            if (playAgainTexture) {
                SDL_Rect playAgainRect = {(WINDOW_WIDTH - playAgainSurface->w) / 2, (WINDOW_HEIGHT - playAgainSurface->h) / 2 + 50, playAgainSurface->w, playAgainSurface->h};
                SDL_RenderCopy(renderer, playAgainTexture, nullptr, &playAgainRect);
                SDL_DestroyTexture(playAgainTexture);

                SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
                SDL_Rect buttonRect = {playAgainRect.x - 10, playAgainRect.y - 10, playAgainRect.w + 20, playAgainRect.h + 20};
                SDL_RenderDrawRect(renderer, &buttonRect);
            }
        }
        if (gameOverSurface) SDL_FreeSurface(gameOverSurface);
        if (playAgainSurface) SDL_FreeSurface(playAgainSurface);
    }
}

void ScoreManager::handleClick(int x, int y, GameState& state) {
    if (state == MENU) {
        SDL_Rect playRect = {(WINDOW_WIDTH - 200) / 2 - 100, (WINDOW_HEIGHT - 200) / 2, 200, 200};
        SDL_Rect instructionRect = {(WINDOW_WIDTH - 200) / 2 + 100, (WINDOW_HEIGHT - 200) / 2, 200, 200};
        SDL_Rect musicRect = {(WINDOW_WIDTH - 400) / 2, (WINDOW_HEIGHT - 200) / 2 + 250, 400, 200};

        if (x >= playRect.x && x <= playRect.x + playRect.w && y >= playRect.y && y <= playRect.y + playRect.h) {
            state = PLAYING;
            reset();
        } else if (x >= instructionRect.x && x <= instructionRect.x + instructionRect.w && y >= instructionRect.y && y <= instructionRect.y + instructionRect.h) {
            // Logic cho Instruction (chưa implement, bạn có thể thêm màn hình hướng dẫn)
            std::cout << "Instruction clicked!" << std::endl;
        } else if (x >= musicRect.x && x <= musicRect.x + musicRect.w && y >= musicRect.y && y <= musicRect.y + musicRect.h) {
            toggleMusic();
        }
    } else if (state == GAME_OVER) {
        SDL_Surface* playAgainSurface = TTF_RenderText_Solid(font, "Play Again", textColor);
        if (playAgainSurface) {
            SDL_Rect playAgainRect = {(WINDOW_WIDTH - playAgainSurface->w) / 2, (WINDOW_HEIGHT - playAgainSurface->h) / 2 + 50, playAgainSurface->w, playAgainSurface->h};
            SDL_Rect buttonRect = {playAgainRect.x - 10, playAgainRect.y - 10, playAgainRect.w + 20, playAgainRect.h + 20};
            SDL_FreeSurface(playAgainSurface);
            if (x >= buttonRect.x && x <= buttonRect.x + buttonRect.w && y >= buttonRect.y && y <= buttonRect.y + buttonRect.h) {
                state = PLAYING;
                reset();
            }
        }
    }
}

void ScoreManager::toggleMusic() {
    musicOn = !musicOn;
    // Thêm logic phát/tắt âm thanh nếu cần (ví dụ: sử dụng SDL_mixer)
    std::cout << (musicOn ? "Music On" : "Music Off") << std::endl;
}
