#ifndef SOUND_H
#define SOUND_H

#include <SDL.h>
#include <SDL_mixer.h>
#include <string>
#include "game.h"

class SoundManager {
private:
    Mix_Music* menuMusic;
    Mix_Music* countdownMusic;
    Mix_Music* gameOverMusic;
    Mix_Music* boomSound;
    Mix_Chunk* shootSound;
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
    void toggleMusic(bool enable, GameState state);
    void playMusicForState(GameState state);
    bool isMusicOn() const { return musicOn; }
};

#endif
