#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include "game.h"
#include "sound.h"

// Lớp InstructionManager quản lý giao diện hướng dẫn chơi game
class InstructionManager {
private:
    SDL_Texture* backgroundTexture; // Texture nền hướng dẫn
    SDL_Texture* instructionTexture; // Texture hướng dẫn
    SDL_Texture* backButtonTexture; // Texture nút quay lại
    SoundManager* soundManager; // Quản lý âm thanh
    void loadTextures(SDL_Renderer* renderer); // Load các hình ảnh cần thiết

public:
    InstructionManager(SDL_Renderer* renderer, SoundManager* sound);  // Constructor khởi tạo InstructionManager
    ~InstructionManager(); // Destructor giải phóng InstructionManager
    void render(SDL_Renderer* renderer); // Render giao diện hướng dẫn
    void handleClick(int x, int y, GameState& state); // Hàm xử lý sự kiện click chuột
};

#endif
