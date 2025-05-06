#ifndef SCORE_H
#define SCORE_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
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
    SDL_Texture* backgroundTexture;
    SDL_Texture* playButtonTexture;
    SDL_Texture* instructionButtonTexture;
    SDL_Texture* musicButtonTexture;
    SDL_Texture* musicOffButtonTexture;
    bool musicOn;
    void loadHighScore();
    void saveHighScore();
    void loadTextures(SDL_Renderer* renderer);

public:
    ScoreManager(SDL_Renderer* renderer);
    ~ScoreManager();
    void addScore(int points);
    int getScore() const;
    int getHighScore() const;
    int getLives() const;
    void loseLife();
    void reset();
    void render(SDL_Renderer* renderer, int x, int y, GameState state);
    void handleClick(int x, int y, GameState& state);
    TTF_Font* getFont() const { return font; }
    SDL_Color getTextColor() const { return textColor; }
    bool isMusicOn() const { return musicOn; }
    void toggleMusic();
};

#endif
