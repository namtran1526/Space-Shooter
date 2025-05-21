#ifndef PLAYER_H
#define PLAYER_H

#include "game.h"

//Lớp quản lý đối tượng Player
class Player {
private:
    GameObject playerObj;
    SDL_Texture* texture;
    const int speed = PLAYER_SPEED;

public:
    Player(SDL_Renderer* renderer);
    ~Player();
    void update(const Uint8* keyboardState); // Cập nhật vị trí Player dựa trên bàn phím
    void render(SDL_Renderer* renderer);
    void resetPosition(); // Đặt lại vị trí Player về vị trí ban đầu sau khi chết
    GameObject& getObject() { return playerObj; } // Trả về tham chiếu để đảm bảo không tạo bản sao
};

#endif
