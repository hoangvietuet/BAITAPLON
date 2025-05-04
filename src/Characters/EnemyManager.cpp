#include "EnemyManager.h"
#include <iostream>

// Thêm một quái vật vào danh sách
void EnemyManager::AddEnemy(Enemy* enemy) {
    m_Enemies.push_back(enemy);
}

// Cập nhật tất cả quái vật
void EnemyManager::UpdateEnemies(float dt) {
    for (auto enemy : m_Enemies) {
        enemy->Update(dt);
    }
}

// Vẽ tất cả quái vật
void EnemyManager::DrawEnemies() {
    for (auto enemy : m_Enemies) {
        enemy->Draw();
    }
}

// Xoá tất cả quái vật
void EnemyManager::Clean() {
    for (auto enemy : m_Enemies) {
        enemy->Clean();
        delete enemy;
    }
    m_Enemies.clear();
}
