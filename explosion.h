#ifndef EXPLOSION_H
#define EXPLOSION_H

#include "game.h"
#include "sound.h"


// Lớp ExplosionManager quản lý hiệu ứng nổ trong trò chơi
class ExplosionManager {
private:
    Explosion explosion; // Đối tượng vụ nổ
    SDL_Texture* texture; // Texture của vụ nổ
    SoundManager* soundManager; 

public:
    ExplosionManager(SDL_Renderer* renderer, SoundManager* sound);  // Constructor khởi tạo ExplosionManager
    ~ExplosionManager();  // Destructor giải phóng ExplosionManager
    void update(); //Cập nhật trạng thái vụ nổ (chuyển khung hình theo thời gian)
    void trigger(int x, int y); // Kích hoạt vụ nổ tại vị trí (x, y)
    void render(SDL_Renderer* renderer); // Render vụ nổ
    bool isActive() const { return explosion.active; } // Kiểm tra xem vụ nổ có đang xảy ra không
};

#endif
