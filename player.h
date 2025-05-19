#ifndef PLAYER_H
#define PLAYER_H

#include "game.h"

//Lớp quản lý đối tượng Player
class Player {
private:
    GameObject playerObj; // Đối tượng đại diện cho Player
    SDL_Texture* texture; // Texture của Player
    const int speed = PLAYER_SPEED; // Tốc độ di chuyển của Player

public:
    Player(SDL_Renderer* renderer);// Constructor khởi tạo Player
    ~Player(); // Destructor giải phóng Player
    void update(const Uint8* keyboardState); // Cập nhật vị trí Player dựa trên bàn phím
    void render(SDL_Renderer* renderer); // render Player lên màn hình
    void resetPosition(); // Đặt lại vị trí Player về vị trí ban đầu sau khi chết
    GameObject& getObject() { return playerObj; } // Trả về tham chiếu để đảm bảo không tạo bản sao
};

#endif
