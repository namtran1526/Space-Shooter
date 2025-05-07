#ifndef SCORE_H
#define SCORE_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include <iostream>
#include "game.h"

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
    int getScore() const;
    int getHighScore() const;
    int getLives() const;
    void loseLife();
    void reset();
    void render(SDL_Renderer* renderer, int x, int y, GameState state, SDL_Texture* liveTexture);
    TTF_Font* getFont() const { return font; }
    SDL_Color getTextColor() const { return textColor; }
};

#endif
