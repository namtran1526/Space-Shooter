#ifndef ENEMY_H
#define ENEMY_H

#include "game.h"
#include <vector>
#include "score.h"


// Lớp quản lí các thiên thạch
class EnemyManager {
private:
    std::vector<GameObject> enemies; // danh sách enemy hiện tại
    SDL_Texture* texture; // texture để hiển thị enemy
    Uint32 lastSpawnTime; // thời gian lần cuối sinh enemy
    const Uint32 SPAWN_INTERVAL = 1000; // khoảng thời gian giữa mỗi lần xuất hiện kẻ thù

public:
    EnemyManager(SDL_Renderer* renderer); // Constructor khởi tạo enemy
    ~EnemyManager(); // Destructor giải phóng texture
    void update(ScoreManager& scoreManager, const GameObject& player); // kiểm tra va chạm và di chuyển enemy xuống dưới
    void spawn(Uint32 currentTime); // sinh ra enemy mới dựa trên thời gian hiện tại
    void render(SDL_Renderer* renderer); // vẽ enemy lên màn hình
    std::vector<GameObject>& getEnemies() { return enemies; } // trả về danh sách enemy để các đối tượng khác có thể kiểm tra va chạm
};

#endif
