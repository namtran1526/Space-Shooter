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
        TTF_Quit();
        return;
    }
    textColor = {255, 255, 255, 255};
    loadHighScore();
}

// Destructor giải phóng ScoreManager
ScoreManager::~ScoreManager() {
    saveHighScore();
    if (font) TTF_CloseFont(font);
    TTF_Quit();
}

// Hàm tải điểm cao từ file
void ScoreManager::loadHighScore() {
    std::ifstream file(highScoreFile);
    if (file.is_open()) {
        file >> highScore;
        file.close();
    }
}

// Hàm lưu điểm cao vào file
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

// Hàm thêm điểm
void ScoreManager::addScore(int points) {
    score += points;
}

// Hàm trả về điểm số hiện tại
int ScoreManager::getScore() const {
    return score;
}

// Hàm trả về điểm cao nhất
int ScoreManager::getHighScore() const {
    return highScore;
}

// Hàm trả về số mạng còn lại
int ScoreManager::getLives() const {
    return lives;
}

// Hàm giảm số mạng
void ScoreManager::loseLife() {
    if (lives > 0) lives--;
}

// Hàm đặt lại điểm số và số mạng
void ScoreManager::reset() {
    score = 0;
    lives = 3;
}

// Hàm render điểm và số 
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
            }
            if (scoreTexture) SDL_DestroyTexture(scoreTexture);
            if (highScoreTexture) SDL_DestroyTexture(highScoreTexture);
        }
        if (scoreSurface) SDL_FreeSurface(scoreSurface);
        if (highScoreSurface) SDL_FreeSurface(highScoreSurface);

        // Hiển thị số mạng (lives)
        if (liveTexture) {
            const int heartSize = 30; // Kích thước 30x30
            const int heartSpacing = 5;
            for (int i = 0; i < lives && i < 3; ++i) {
                SDL_Rect liveRect = {x + i * (heartSize + heartSpacing), y + 70, heartSize, heartSize};
                SDL_RenderCopy(renderer, liveTexture, nullptr, &liveRect);
            }
        }
    }
}
