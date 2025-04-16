#include "core/ObjectManager.h"
#include "core/Bullet.h"
#include "core/GameObject.h"
#include "core/RectangleCollider.h"
#include "core/CircleCollider.h"

#ifndef COLLISION_MANADGER_H
#define COLLISION_MANADGER_H

#include <glm/glm.hpp>

struct CollisionPair {
    std::shared_ptr<Bullet> bullet;
    std::shared_ptr<GameObject> object;

    CollisionPair()
    : bullet(nullptr),
      object(nullptr) {
    }

    CollisionPair(const std::shared_ptr<Bullet>& bullet, const std::shared_ptr<GameObject>& gameObject)
      : bullet(bullet),
        object(gameObject) {
    }
};

class CollisionManager {
private:
    ObjectManager& objectManager;
    std::vector<CollisionPair> enemyAndPlayerBulletsCollisions;
    CollisionPair playerAndEnemyBulletCollision;

    static bool checkCollision(glm::vec2 aPos, glm::vec2 bPos, std::shared_ptr<CircleCollider> a, std::shared_ptr<CircleCollider> b);

    static bool checkCollision(glm::vec2 aPos, glm::vec2 bPos, std::shared_ptr<RectangleCollider> a, std::shared_ptr<RectangleCollider> b);

    static bool checkCollision(glm::vec2 aPos, glm::vec2 bPos, std::shared_ptr<CircleCollider> a, std::shared_ptr<RectangleCollider> b);

    static bool checkCollision(glm::vec2 aPos, glm::vec2 bPos, std::shared_ptr<RectangleCollider> a, std::shared_ptr<CircleCollider> b);

    static bool checkCollision(glm::vec2 aPos, glm::vec2 bPos, std::shared_ptr<Collider> a, std::shared_ptr<Collider> b);
public:
    CollisionManager();

    CollisionManager(ObjectManager& ObjectManager);

    void SetObjectManager(const ObjectManager& objectManager) {
        this->objectManager = objectManager;
    }

    const std::vector<CollisionPair>& GetPlayerBulletToEnemyCollisions();

    const CollisionPair& EnemyBulletToPlayerCollision();
};

#endif // COLLISION_MANADGER_H