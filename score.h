#ifndef SCORE_H
#define SCORE_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include <iostream>
#include "game.h"

// Lớp ScoreManager quản lý điểm số, điểm cao và số mạng của người chơi
class ScoreManager {
private:
    int score; // Điểm số hiện tại
    int highScore; // Điểm cao nhất
    int lives; // Số mạng còn lại
    TTF_Font* font; // Phông chữ để hiển thị điểm số
    SDL_Color textColor; // Màu sắc của văn bản
    const std::string highScoreFile = "highscore.txt"; // file lưu điểm cao
    void loadHighScore(); // Tải điểm cao từ file
    void saveHighScore(); // Lưu điểm cao vào file

public:
    ScoreManager(); // Constructor khởi tạo ScoreManager
    ~ScoreManager(); // Destructor giải phóng ScoreManager
    void addScore(int points);  // Hàm thêm điểm
    int getScore() const; // Trả về điểm số hiện tại
    int getHighScore() const; // Trả về điểm cao nhất
    int getLives() const; // Trả về số mạng còn lại
    void loseLife(); // Giảm số mạng
    void reset(); // Đặt lại điểm số và số mạng
    void render(SDL_Renderer* renderer, int x, int y, GameState state, SDL_Texture* liveTexture); // Render điểm và số mạng lên
    
    // Trả về font chữ và màu sắc văn bản
    TTF_Font* getFont() const { return font; } 
    SDL_Color getTextColor() const { return textColor; }
};

#endif
