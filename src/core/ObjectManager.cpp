#include "core/ObjectManager.h"

#include "core/Enemy.h"
#include "core/Bullet.h"
#include "core/Player.h"

ObjectManager::ObjectManager()
  : enemyPool(1),
    enemyBulletPool(1),
    playerBulletPool(1),
    player() {
};

void ObjectManager::SetPlayer(std::shared_ptr<Player> player) {
    this->player = player;
    player->SetObjectManager(shared_from_this());
}

std::shared_ptr<Player> ObjectManager::GetPlayer() {
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

std::shared_ptr<Bullet> ObjectManager::GetFreePlayerBullet() {
    return playerBulletPool.GetFreeElement();
}

void ObjectManager::FixedUpdate(float fixedDeltaTime) {
    for (std::shared_ptr<GameObject> object : GetEnemies()) {
        object->FixedUpdate(fixedDeltaTime);
    }

    for (std::shared_ptr<GameObject> object : GetEnemyBullets()) {
        object->FixedUpdate(fixedDeltaTime);
    }

    for (std::shared_ptr<GameObject> object : GetPlayerBullets()) {
        object->FixedUpdate(fixedDeltaTime);
    }

    GetPlayer()->FixedUpdate(fixedDeltaTime);
}

void ObjectManager::Update(float deltaTime) {
    for (std::shared_ptr<GameObject> object : GetEnemies()) {
        if (object->GetActive()) {
            object->Update(deltaTime);
        }
    }

    for (std::shared_ptr<GameObject> object : GetEnemyBullets()) {
        if (object->GetActive()) {
            object->Update(deltaTime);
        }
    }

    for (std::shared_ptr<GameObject> object : GetPlayerBullets()) {
        if (object->GetActive()) {
            object->Update(deltaTime);
        }
    }

    GetPlayer()->Update(deltaTime);
}

void ObjectManager::Draw(sf::RenderTarget& target) {
    for (std::shared_ptr<GameObject> object : GetEnemies()) {
        if (object->GetActive()) {
            object->Draw(target);
        }
    }

    for (std::shared_ptr<GameObject> object : GetEnemyBullets()) {
        if (object->GetActive()) {
            object->Draw(target);
        }
    }

    for (std::shared_ptr<GameObject> object : GetPlayerBullets()) {
        if (object->GetActive()) {
            object->Draw(target);
        }
    }

    GetPlayer()->Draw(target);
}