#include "sound.h"
#include <iostream>

SoundManager::SoundManager() : menuMusic(nullptr), countdownMusic(nullptr), gameOverMusic(nullptr), boomSound(nullptr),
                              shootSound(nullptr), musicOn(true) {
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cout << "Error: Could not initialize SDL_mixer! " << Mix_GetError() << std::endl;
        return;
    }

    menuMusic = Mix_LoadMUS("music/Menu.mp3");
    if (!menuMusic) {
        std::cout << "Warning: Could not load music/Menu.mp3! " << Mix_GetError() << std::endl;
    }

    countdownMusic = Mix_LoadMUS("music/Countdown.mp3");
    if (!countdownMusic) {
        std::cout << "Warning: Could not load music/Countdown.mp3! " << Mix_GetError() << std::endl;
    }

    gameOverMusic = Mix_LoadMUS("music/GameOver.mp3");
    if (!gameOverMusic) {
        std::cout << "Warning: Could not load music/GameOver.mp3! " << Mix_GetError() << std::endl;
    }

    boomSound = Mix_LoadMUS("music/Boom.mp3");
    if (!boomSound) {
        std::cout << "Warning: Could not load music/Boom.mp3! " << Mix_GetError() << std::endl;
    }

    shootSound = Mix_LoadWAV("music/Shoot.wav");
    if (!shootSound) {
        std::cout << "Warning: Could not load music/Shoot.wav! " << Mix_GetError() << std::endl;
    }
}

SoundManager::~SoundManager() {
    if (menuMusic) Mix_FreeMusic(menuMusic);
    if (countdownMusic) Mix_FreeMusic(countdownMusic);
    if (gameOverMusic) Mix_FreeMusic(gameOverMusic);
    if (boomSound) Mix_FreeMusic(boomSound);
    if (shootSound) Mix_FreeChunk(shootSound);
    Mix_CloseAudio();
}

void SoundManager::playMenuMusic() {
    if (musicOn && menuMusic && !Mix_PlayingMusic()) {
        Mix_PlayMusic(menuMusic, -1); // Lặp vô hạn
        std::cout << "Playing Menu music" << std::endl;
    }
}

void SoundManager::playCountdownMusic() {
    if (musicOn && countdownMusic && !Mix_PlayingMusic()) {
        Mix_PlayMusic(countdownMusic, 0); //Phát 1 lần
        std::cout << "Playing Countdown music" << std::endl;
    }
}

void SoundManager::playGameOverMusic() {
    if (musicOn && gameOverMusic && !Mix_PlayingMusic()) {
        Mix_PlayMusic(gameOverMusic, -1); // Lặp vô hạn
        std::cout << "Playing GameOver music" << std::endl;
    }
}

void SoundManager::playBoomSound() {
    if (musicOn && boomSound) {
        Mix_PlayMusic(boomSound, 0); // Phát một lần
        std::cout << "Playing Boom sound" << std::endl;
    }
}

void SoundManager::playShootSound() {
    if (musicOn && shootSound) {
        Mix_PlayChannel(-1, shootSound, 0); // Phát một lần, dùng channel tự do
        std::cout << "Playing Shoot sound" << std::endl;
    }
}

void SoundManager::stopMusic() {
    if (Mix_PlayingMusic()) {
        Mix_HaltMusic();
        std::cout << "Stopped music" << std::endl;
    }
    Mix_HaltChannel(-1); // Dừng tất cả channel âm thanh
}

void SoundManager::toggleMusic(bool enable, GameState state) {
    musicOn = enable;
    if (!musicOn) {
        stopMusic();
        std::cout << "Music disabled" << std::endl;
    } else {
        playMusicForState(state); // Phát nhạc tương ứng khi bật
        std::cout << "Music enabled" << std::endl;
    }
}

void SoundManager::playMusicForState(GameState state) {
    stopMusic(); // Dừng tất cả âm thanh trước khi phát nhạc mới
    if (!musicOn) return;
    switch (state) {
        case MENU:
        case INSTRUCTIONS:
            playMenuMusic();
            break;
        case COUNTDOWN:
            playCountdownMusic();
            break;
        case GAME_OVER:
            playGameOverMusic();
            break;
        default:
            // Không phát nhạc cho PLAYING, PAUSED, EXPLODING
            break;
    }
}
