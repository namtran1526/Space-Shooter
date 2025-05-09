#ifndef PLAYER_H
#define PLAYER_H

#include "game.h"

class Player {
private:
    GameObject playerObj;
    SDL_Texture* texture;
    const int speed = PLAYER_SPEED;

public:
    Player(SDL_Renderer* renderer);
    ~Player();
    void update(const Uint8* keyboardState);
    void render(SDL_Renderer* renderer);
    void resetPosition();
    GameObject& getObject() { return playerObj; } // Trả về tham chiếu để đảm bảo không tạo bản sao
};

#endif
