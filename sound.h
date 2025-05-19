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
    bool musicOn; // Biến kiểm tra âm thanh có bật không

public:
    SoundManager(); // Constructor khởi tạo SoundManager
    ~SoundManager(); // Destructor giải phóng SoundManager
    void playMenuMusic(); // Phát nhạc nền menu
    void playCountdownMusic(); // Phát nhạc nền đếm ngược
    void playGameOverMusic(); // Phát nhạc nền game over
    void playBoomSound(); // Phát âm thanh nổ
    void playShootSound(); // Phát âm thanh bắn
    void stopMusic(); // Dừng nhạc
    void toggleMusic(bool enable, GameState state); // Bật/tắt nhạc
    void playMusicForState(GameState state); // Phát nhạc tương ứng với trạng thái trò chơi
    bool isMusicOn() const { return musicOn; } // Kiểm tra xem nhạc có đang bật không
};

#endif
