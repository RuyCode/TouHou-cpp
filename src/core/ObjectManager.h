#include "core/ObjectPool.h"

#include "core/Enemy.h"
#include "core/Bullet.h"
#include "core/Player.h"

#ifndef OBJECT_MANAGER_H
#define OBJECT_MANAGER_H

class ObjectManager {
private:
    ObjectPool<Enemy> enemyPool;
    ObjectPool<Bullet> enemyBulletPool;

    Player player;
    ObjectPool<Bullet> playerBulletPool;

public:

    ObjectManager();

    void SetPlayer(const Player& player);
    
    Player& GetPlayer();

    std::vector<std::shared_ptr<Enemy>>& GetEnemies();

    std::vector<std::shared_ptr<Bullet>>& GetEnemyBullets();

    std::vector<std::shared_ptr<Bullet>>& GetPlayerBullets();
};

#endif // OBJECT_MANAGER_H