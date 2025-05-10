#include "score.h"
#include "game.h"
#include <fstream>
#include <sstream>

ScoreManager::ScoreManager() : score(0), highScore(0), lives(3), font(nullptr) {
    if (TTF_Init() < 0) {
        std::cout << "Error: TTF_Init failed! " << TTF_GetError() << std::endl;
        return;
    }
    font = TTF_OpenFont("resources/VNI-Lithos.TTF", 24); // Sử dụng VNI-Lithos.TTF
    if (!font) {
        std::cout << "Error: Could not load font at 'resources/VNI-Lithos.TTF'! SDL_ttf Error: " << TTF_GetError() << std::endl;
        std::cout << "Please ensure the font file exists in the resources folder and the path is correct." << std::endl;
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

    // Chỉ render điểm số và số mạng khi không ở trạng thái MENU, GAME_OVER, hoặc INSTRUCTIONS
    if (state != MENU && state != GAME_OVER && state != INSTRUCTIONS) {
        // Hiển thị điểm số và điểm cao nhất
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
                std::cout << "Rendered Score: " << scoreText << " and High Score: " << highScoreText << " at state: " << static_cast<int>(state) << std::endl;
            }
            if (scoreTexture) SDL_DestroyTexture(scoreTexture);
            if (highScoreTexture) SDL_DestroyTexture(highScoreTexture);
        }
        if (scoreSurface) SDL_FreeSurface(scoreSurface);
        if (highScoreSurface) SDL_FreeSurface(highScoreSurface);

        // Hiển thị số mạng (lives) với kích thước lớn hơn và dịch xuống dưới
        if (liveTexture) {
            const int heartSize = 30; // Kích thước 30x30
            const int heartSpacing = 5;
            for (int i = 0; i < lives && i < 3; ++i) {
                SDL_Rect liveRect = {x + i * (heartSize + heartSpacing), y + 70, heartSize, heartSize};
                SDL_RenderCopy(renderer, liveTexture, nullptr, &liveRect);
            }
            std::cout << "Rendered Lives: " << lives << " at state: " << static_cast<int>(state) << std::endl;
        }
    }
}
