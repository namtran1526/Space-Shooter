#include "sound.h"
#include <iostream>

// Constructor khởi tạo SoundManager
SoundManager::SoundManager() : menuMusic(nullptr), countdownMusic(nullptr), gameOverMusic(nullptr), boomSound(nullptr),
                              shootSound(nullptr), musicOn(true) {
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cout << "Error: Could not initialize SDL_mixer! " << Mix_GetError() << std::endl;
        return;
    }
    menuMusic = Mix_LoadMUS("music/Menu.mp3");
    countdownMusic = Mix_LoadMUS("music/Countdown.mp3");
    gameOverMusic = Mix_LoadMUS("music/GameOver.mp3");
    boomSound = Mix_LoadMUS("music/Boom.mp3");
    shootSound = Mix_LoadWAV("music/Shoot.wav");
}

// Destructor giải phóng SoundManager
SoundManager::~SoundManager() {
    if (menuMusic) Mix_FreeMusic(menuMusic);
    if (countdownMusic) Mix_FreeMusic(countdownMusic);
    if (gameOverMusic) Mix_FreeMusic(gameOverMusic);
    if (boomSound) Mix_FreeMusic(boomSound);
    if (shootSound) Mix_FreeChunk(shootSound);
    Mix_CloseAudio();
}

// Phát nhạc với trạng tháI MENU
void SoundManager::playMenuMusic() {
    if (musicOn && menuMusic && !Mix_PlayingMusic()) {
        Mix_PlayMusic(menuMusic, -1); // Lặp vô hạn
        std::cout << "Playing Menu music" << std::endl;
    }
}
// Phát nhạc với trạng thái COUNTDOWN
void SoundManager::playCountdownMusic() {
    if (musicOn && countdownMusic && !Mix_PlayingMusic()) {
        Mix_PlayMusic(countdownMusic, 0); //Phát 1 lần
        std::cout << "Playing Countdown music" << std::endl;
    }
}
// Phát nhạc với trạng thái GAME_OVER
void SoundManager::playGameOverMusic() {
    if (musicOn && gameOverMusic && !Mix_PlayingMusic()) {
        Mix_PlayMusic(gameOverMusic, -1); // Lặp vô hạn
        std::cout << "Playing GameOver music" << std::endl;
    }
}
// Phát âm thanh nổ
void SoundManager::playBoomSound() {
    if (musicOn && boomSound) {
        Mix_PlayMusic(boomSound, 0); // Phát một lần
        std::cout << "Playing Boom sound" << std::endl;
    }
}
// Phát âm thanh bắn
void SoundManager::playShootSound() {
    if (musicOn && shootSound) {
        Mix_PlayChannel(-1, shootSound, 0); // Phát một lần, dùng channel tự do
        std::cout << "Playing Shoot sound" << std::endl;
    }
}
// Dừng tất cả âm thanh
void SoundManager::stopMusic() {
    if (Mix_PlayingMusic()) {
        Mix_HaltMusic();
        std::cout << "Stopped music" << std::endl;
    }
    Mix_HaltChannel(-1);
}

// Bật/tắt nhạc
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

// Phát nhạc tương ứng với trạng thái trò chơi
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
