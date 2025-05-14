#include "core/ObjectPool.h"

class Enemy;
class Player;
class Bullet;

#ifndef OBJECT_MANAGER_H
#define OBJECT_MANAGER_H

class ObjectManager : public std::enable_shared_from_this<ObjectManager> {
private:
    ObjectPool<Enemy> enemyPool;
    ObjectPool<Bullet> enemyBulletPool;

    std::shared_ptr<Player> player;
    ObjectPool<Bullet> playerBulletPool;

public:

    ObjectManager();

    void SetPlayer(std::shared_ptr<Player> player);
    
    std::shared_ptr<Player> GetPlayer();

    std::vector<std::shared_ptr<Enemy>>& GetEnemies();

    std::vector<std::shared_ptr<Bullet>>& GetEnemyBullets();

    std::vector<std::shared_ptr<Bullet>>& GetPlayerBullets();

    std::shared_ptr<Bullet> GetFreePlayerBullet();

    void FixedUpdate(float fixedDeltaTime);

    void Update(float deltaTime);

    void Draw(sf::RenderTarget& target);
};

#endif // OBJECT_MANAGER_H