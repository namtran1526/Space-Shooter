#ifndef BULLET_H
#define BULLET_H

#include "game.h"
#include <vector>
#include "score.h"
#include "sound.h"

// Lớp quản lý đạn do Player bắn ra
class BulletManager {
private:
    std::vector<GameObject> bullets; // danh sách bullet hiện tại
    SDL_Texture* texture; // texture của viên đạn
    Uint32 lastShotTime; // thời điểm cuối bắn đạn
    SoundManager* soundManager; // quản lý âm thanh để phát âm thanh bắn

public:
    BulletManager(SDL_Renderer* renderer, SoundManager* sound); // Constructor khởi tạo texture và âm thanh
    ~BulletManager(); // Destructor giải phóng texture
    void update(ScoreManager& scoreManager); // cập nhật vị trí bullet, kiểm tra đã đi ra khỏi màn hình
    void shoot(int x, int y, Uint32 currentTime, Uint32 cooldown); // bắn đạn ra nếu hết thời gian cooldown
    void render(SDL_Renderer* renderer); // hiển thị bullet
    std::vector<GameObject>& getBullets() { return bullets; } // trả về danh sách bullet để kiểm tra va chạm với enemy
};

#endif
