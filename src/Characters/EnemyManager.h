#ifndef ENEMYMANAGER_H
#define ENEMYMANAGER_H

#include "Enemy.h"
#include <vector>

class EnemyManager {
private:
    std::vector<Enemy*> m_Enemies;  // Danh sách các quái vật

public:
    EnemyManager() {}
    ~EnemyManager();

    // Thêm một quái vật vào danh sách
    void AddEnemy(Enemy* enemy);

    // Cập nhật tất cả quái vật
    void UpdateEnemies(float dt);

    // Vẽ tất cả quái vật
    void DrawEnemies();

    // Xoá tất cả quái vật
    void Clean();
};

#endif
