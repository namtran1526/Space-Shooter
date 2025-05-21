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
    int score;
    int highScore;
    int lives;
    TTF_Font* font;
    SDL_Color textColor;
    const std::string highScoreFile = "highscore.txt";
    void loadHighScore();
    void saveHighScore();

public:
    ScoreManager();
    ~ScoreManager();
    void addScore(int points);
    int getScore() const; // Trả về điểm số hiện tại
    int getHighScore() const; // Trả về điểm cao nhất
    int getLives() const; // Trả về số mạng còn lại
    void loseLife(); // Giảm số mạng
    void reset(); // Đặt lại điểm số và số mạng
    void render(SDL_Renderer* renderer, int x, int y, GameState state, SDL_Texture* liveTexture);

    TTF_Font* getFont() const { return font; }
    SDL_Color getTextColor() const { return textColor; }
};

#endif
