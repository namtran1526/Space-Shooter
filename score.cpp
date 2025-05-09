#include "score.h"
#include <fstream>
#include <sstream>

ScoreManager::ScoreManager() : score(0), highScore(0), lives(3), font(nullptr) {
    if (TTF_Init() < 0) {
        std::cout << "Error: TTF_Init failed! " << TTF_GetError() << std::endl;
        return;
    }
    font = TTF_OpenFont("C:\\Windows\\Fonts\\arial.ttf", 24); // Sử dụng đường dẫn Windows
    if (!font) {
        std::cout << "Error: Could not load font at 'C:\\Windows\\Fonts\\arial.ttf'! SDL_ttf Error: " << TTF_GetError() << std::endl;
        std::cout << "Please ensure the font file exists or adjust the path if needed." << std::endl;
        TTF_Quit();
        return;
    }
    textColor = {255, 255, 255, 255};
    loadHighScore();
}

ScoreManager::~ScoreManager() {
    saveHighScore();
    if (font) TTF_CloseFont(font);
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

void ScoreManager::render(SDL_Renderer* renderer, int x, int y, GameState state, SDL_Texture* liveTexture) {
    if (!font) {
        std::cout << "Error: Font not loaded, cannot render text!" << std::endl;
        return;
    }

    if (state == PLAYING || state == PAUSED) {
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
                std::cout << "Rendered Score: " << scoreText << " and High Score: " << highScoreText << " at state: " << (state == PLAYING ? "PLAYING" : "PAUSED") << std::endl;
            }
            if (scoreTexture) SDL_DestroyTexture(scoreTexture);
            if (highScoreTexture) SDL_DestroyTexture(highScoreTexture);
        }
        if (scoreSurface) SDL_FreeSurface(scoreSurface);
        if (highScoreSurface) SDL_FreeSurface(highScoreSurface);

        if (liveTexture) {
            const int heartSize = 20;
            const int heartSpacing = 5;
            for (int i = 0; i < lives && i < 3; ++i) {
                SDL_Rect liveRect = {x + i * (heartSize + heartSpacing), y + 60, heartSize, heartSize};
                SDL_RenderCopy(renderer, liveTexture, nullptr, &liveRect);
            }
            std::cout << "Rendered Lives: " << lives << " at state: " << (state == PLAYING ? "PLAYING" : "PAUSED") << std::endl;
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
            SDL_FreeSurface(gameOverSurface);
            SDL_FreeSurface(playAgainSurface);
        }
    }
}
