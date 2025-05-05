#include "score.h"
#include <fstream>
#include <sstream>
#include <iostream>

ScoreManager::ScoreManager() : score(0), highScore(0), lives(3) {
    TTF_Init();
    font = TTF_OpenFont("DungeonFont.ttf", 24);
    if (!font) {
        font = TTF_OpenFont("C:\\Windows\\Fonts\\arial.ttf", 24); // Đường dẫn font mặc định Windows
        if (!font) {
            std::cout << "Error: Could not load font! " << TTF_GetError() << std::endl;
        }
    }
    textColor = {255, 255, 255, 255}; // Màu trắng
    loadHighScore();
}

ScoreManager::~ScoreManager() {
    saveHighScore();
    if (font) {
        TTF_CloseFont(font);
    }
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
    if (lives > 0) {
        lives--;
    }
}

void ScoreManager::render(SDL_Renderer* renderer, int x, int y) {
    if (!font) return;

    std::string scoreText = "Score: " + std::to_string(score);
    std::string highScoreText = "High Score: " + std::to_string(highScore);

    SDL_Surface* scoreSurface = TTF_RenderText_Solid(font, scoreText.c_str(), textColor);
    SDL_Surface* highScoreSurface = TTF_RenderText_Solid(font, highScoreText.c_str(), textColor);

    if (scoreSurface && highScoreSurface) {
        SDL_Texture* scoreTexture = SDL_CreateTextureFromSurface(renderer, scoreSurface);
        SDL_Texture* highScoreTexture = SDL_CreateTextureFromSurface(renderer, highScoreSurface);

        SDL_Rect scoreRect = {x, y, scoreSurface->w, scoreSurface->h};
        SDL_Rect highScoreRect = {x, y + 30, highScoreSurface->w, highScoreSurface->h};

        SDL_RenderCopy(renderer, scoreTexture, nullptr, &scoreRect);
        SDL_RenderCopy(renderer, highScoreTexture, nullptr, &highScoreRect);

        SDL_DestroyTexture(scoreTexture);
        SDL_DestroyTexture(highScoreTexture);
    }

    if (scoreSurface) SDL_FreeSurface(scoreSurface);
    if (highScoreSurface) SDL_FreeSurface(highScoreSurface);

    // Vẽ 3 trái tim biểu thị mạng
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Màu đỏ
    const int heartSize = 20;
    const int heartSpacing = 5;
    for (int i = 0; i < lives && i < 3; ++i) {
        SDL_Rect heartRect = {x + i * (heartSize + heartSpacing), y + 60, heartSize, heartSize};
        SDL_RenderFillRect(renderer, &heartRect); // Vẽ hình chữ nhật đơn giản làm trái tim
    }
}
