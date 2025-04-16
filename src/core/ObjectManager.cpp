#include "core/ObjectManager.h"

ObjectManager::ObjectManager()
  : enemyPool(1),
    enemyBulletPool(1),
    playerBulletPool(1),
    player() {
};

void ObjectManager::SetPlayer(const Player& player) {
    this->player = player;
}

Player& ObjectManager::GetPlayer() {
    return player;
}

std::vector<std::shared_ptr<Enemy>>& ObjectManager::GetEnemies() {
    return enemyPool.GetPool();
}

std::vector<std::shared_ptr<Bullet>>& ObjectManager::GetEnemyBullets() {
    return enemyBulletPool.GetPool();
}

std::vector<std::shared_ptr<Bullet>>& ObjectManager::GetPlayerBullets() {
    return playerBulletPool.GetPool();
}