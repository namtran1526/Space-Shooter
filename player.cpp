#include "player.h"
#include <SDL_image.h>
#include <iostream>

Player::Player(SDL_Renderer* renderer) : texture(nullptr) {
    SDL_Surface* playerSurface = IMG_Load("resources/Player.png");
    if (playerSurface) {
        texture = SDL_CreateTextureFromSurface(renderer, playerSurface);
        SDL_FreeSurface(playerSurface);
    } else {
        std::cout << "Warning: Could not load Player.png! Using fallback. " << IMG_GetError() << std::endl;
    }
    playerObj = {{WINDOW_WIDTH / 2 - PLAYER_WIDTH / 2, WINDOW_HEIGHT - PLAYER_HEIGHT - 10, PLAYER_WIDTH, PLAYER_HEIGHT}, true, texture};
    std::cout << "Player initialized at x=" << playerObj.rect.x << ", y=" << playerObj.rect.y << std::endl;
}

Player::~Player() {
    if (texture) SDL_DestroyTexture(texture);
}

void Player::update(const Uint8* keyboardState) {
    if (!playerObj.active) return;

    int dx = 0;
    if (keyboardState[SDL_SCANCODE_LEFT]) dx -= speed;
    if (keyboardState[SDL_SCANCODE_RIGHT]) dx += speed;

    playerObj.rect.x += dx;
    if (playerObj.rect.x < 0) playerObj.rect.x = 0;
    if (playerObj.rect.x > WINDOW_WIDTH - playerObj.rect.w) playerObj.rect.x = WINDOW_WIDTH - playerObj.rect.w;

    std::cout << "Player updated position: x=" << playerObj.rect.x << ", y=" << playerObj.rect.y << ", active=" << playerObj.active << std::endl;
}

void Player::render(SDL_Renderer* renderer) {
    if (playerObj.active) {
        if (playerObj.texture) {
            SDL_RenderCopy(renderer, playerObj.texture, nullptr, &playerObj.rect);
        } else {
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
            SDL_RenderFillRect(renderer, &playerObj.rect);
        }
    }
}

void Player::resetPosition() {
    playerObj.rect.x = WINDOW_WIDTH / 2 - PLAYER_WIDTH / 2;
    playerObj.rect.y = WINDOW_HEIGHT - PLAYER_HEIGHT - 10;
    playerObj.active = true;
    std::cout << "Player reset to x=" << playerObj.rect.x << ", y=" << playerObj.rect.y << std::endl;
}
