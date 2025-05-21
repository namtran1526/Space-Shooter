#ifndef SOUND_H
#define SOUND_H

#include <SDL.h>
#include <SDL_mixer.h>
#include <string>
#include "game.h"

// Lớp quản lý âm thanh trong trò chơi
class SoundManager {
private:
    Mix_Music* menuMusic; // Nhạc nền menu
    Mix_Music* countdownMusic; // Nhạc nền đếm ngược
    Mix_Music* gameOverMusic; // Nhạc nền game over
    Mix_Music* boomSound; // Âm thanh nổ
    Mix_Chunk* shootSound; // Âm thanh bắn
    bool musicOn;

public:
    SoundManager();
    ~SoundManager();
    void playMenuMusic();
    void playCountdownMusic();
    void playGameOverMusic();
    void playBoomSound();
    void playShootSound();
    void stopMusic();
    void toggleMusic(bool enable, GameState state); // Bật/tắt nhạc
    void playMusicForState(GameState state); // Phát nhạc tương ứng với trạng thái trò chơi
    bool isMusicOn() const { return musicOn; }
};

#endif
