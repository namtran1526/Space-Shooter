#ifndef MENU_H
#define MENU_H

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include "game.h"
#include "sound.h"

// Lớp MenuManager quản lý giao diện menu chính của trò chơi
class MenuManager {
private:
    SDL_Texture* backgroundTexture;  // Texture nền menu
    SDL_Texture* playButtonTexture; // Texture nút Play
    SDL_Texture* instructionButtonTexture; // Texture nút Hướng dẫn
    SDL_Texture* musicButtonTexture;  // Texture nút Âm thanh
    SDL_Texture* pauseButtonTexture; // Texture nút Tạm dừng
    bool musicOn; // Biến kiểm tra âm thanh có bật không
    SDL_Rect musicButtonSrcRect; // Kích thước nguồn của nút âm thanh
    SDL_Rect pauseButtonSrcRect; // Kích thước nguồn của nút tạm dừng
    SoundManager* soundManager;
    void loadTextures(SDL_Renderer* renderer); // Load các hình ảnh cần thiết

public:
    MenuManager(SDL_Renderer* renderer, SoundManager* sound);  // Constructor khởi tạo MenuManager
    ~MenuManager(); // Destructor giải phóng MenuManager
    void render(SDL_Renderer* renderer, GameState state); // Render giao diện menu
    void handleClick(int x, int y, GameState& state); // Hàm xử lý sự kiện click chuột
    bool isMusicOn() const { return musicOn; } // Kiểm tra xem nhạc có đang bật không
};

#endif
