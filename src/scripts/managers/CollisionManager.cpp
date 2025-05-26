#include "scripts/managers/CollisionManager.h"

#include "scripts/entities/Enemy.h"
#include "scripts/entities/Player.h"
#include "scripts/entities/Bullet.h"

#include <limits>

CollisionManager::CollisionManager(std::shared_ptr<ObjectManager> objectManager): objectManager(objectManager){};

bool CollisionManager::checkCollision(glm::vec2 aPos, glm::vec2 bPos, std::shared_ptr<CircleCollider> a, std::shared_ptr<CircleCollider> b) {
    glm::vec2 diff = bPos - aPos;
    return glm::dot(diff, diff) < (a->GetRadius() + b->GetRadius()) * (a->GetRadius() + b->GetRadius());
}

bool CollisionManager::checkCollision(glm::vec2 aPos, glm::vec2 bPos, std::shared_ptr<RectangleCollider> a, std::shared_ptr<RectangleCollider> b) {
    glm::vec2 axes[4] {(*a)[0] - (*a)[1], (*a)[1] - (*a)[2], (*b)[0] - (*b)[1], (*b)[1] - (*b)[2]};

    glm::vec2 axis{};
    
    float maxA{};
    float minA{};

    float maxB{};
    float minB{};

    for (unsigned int t = 0; t < 4; ++t){
        axis = axes[t];

        maxA = -std::numeric_limits<float>::infinity();
        minA = std::numeric_limits<float>::infinity();
    
        maxB = -std::numeric_limits<float>::infinity();
        minB = std::numeric_limits<float>::infinity();

        for (std::size_t i = 0; i < 4; ++i) {
            float projA = glm::dot(axis, aPos + (*a)[i]);
            float projB = glm::dot(axis, bPos + (*b)[i]);

            maxA = projA > maxA ? projA : maxA;
            maxB = projB > maxB ? projB : maxB;

            minA = projA < minA ? projA : minA;
            minB = projB < minB ? projB : minB;
        }

        if (maxA < minB || maxB < minA) {
            return false;
        }
    }

    return true;
}

bool CollisionManager::checkCollision(glm::vec2 aPos, glm::vec2 bPos, std::shared_ptr<CircleCollider> a, std::shared_ptr<RectangleCollider> b) {
    glm::vec2 axes[2] = {(*b)[0] - (*b)[1], (*b)[1] - (*b)[2]};

    glm::vec2 axis{};

    float maxB{};
    float minB{};

    for (unsigned int t = 0; t < 2; ++t){
        axis = axes[t];

        float maxA = glm::dot(axis, aPos + glm::normalize(axis) * a->GetRadius());
        float minA = glm::dot(axis, aPos - glm::normalize(axis) * a->GetRadius());

        if (maxA == minA) {
            std::swap(maxA, minA);
        }

        maxB = -std::numeric_limits<float>::infinity();
        minB = std::numeric_limits<float>::infinity();

        for (std::size_t i = 0; i < 4; ++i) {
            float projB = glm::dot(axis, bPos + (*b)[i]);

            maxB = projB > maxB ? projB : maxB;
            minB = projB < minB ? projB : minB;
        }

        if (maxA < minB || maxB < minA) {
            return false;
        }
    }

    return true;
}

bool CollisionManager::checkCollision(glm::vec2 aPos, glm::vec2 bPos, std::shared_ptr<RectangleCollider> a, std::shared_ptr<CircleCollider> b) {
    return checkCollision(aPos, bPos, b, a);
}

bool CollisionManager::checkCollision(glm::vec2 aPos, glm::vec2 bPos, std::shared_ptr<Collider> a, std::shared_ptr<Collider> b) {
    if (!a || !b) return false;

    if (typeid(*a) == typeid(CircleCollider) && typeid(*b) == typeid(CircleCollider)) {
        auto ca = std::dynamic_pointer_cast<CircleCollider>(a);
        auto cb = std::dynamic_pointer_cast<CircleCollider>(b);
        if (!ca || !cb) return false; // Extra safety
        return checkCollision(aPos, bPos, ca, cb);
    } else if (typeid(*a) == typeid(CircleCollider) && typeid(*b) == typeid(RectangleCollider)) {
        auto ca = std::dynamic_pointer_cast<CircleCollider>(a);
        auto cb = std::dynamic_pointer_cast<RectangleCollider>(b);
        if (!ca || !cb) return false;
        return checkCollision(aPos, bPos, ca, cb);
    } else if (typeid(*a) == typeid(RectangleCollider) && typeid(*b) == typeid(CircleCollider)) {
        auto ca = std::dynamic_pointer_cast<RectangleCollider>(a);
        auto cb = std::dynamic_pointer_cast<CircleCollider>(b);
        if (!ca || !cb) return false;
        return checkCollision(aPos, bPos, ca, cb);
    } else if (typeid(*a) == typeid(RectangleCollider) && typeid(*b) == typeid(RectangleCollider)) {
        auto ca = std::dynamic_pointer_cast<RectangleCollider>(a);
        auto cb = std::dynamic_pointer_cast<RectangleCollider>(b);
        if (!ca || !cb) return false;
        return checkCollision(aPos, bPos, ca, cb);
    }

    return false;
}

const std::vector<CollisionPair>& CollisionManager::GetPlayerBulletToEnemyCollisions() {
    enemyAndPlayerBulletsCollisions.clear();

    auto bullets = objectManager->GetPlayerBullets();
    auto enemies = objectManager->GetEnemies();

    for (const std::shared_ptr<Bullet>& bullet : bullets) {
        if (!bullet->GetActive()) {
            continue;
        }

        float bulletY = bullet->GetPosition().y;
        float bulletCollisionDistance = bullet->GetCollider()->GetMaxCollisionDistance();

        for (const auto& enemy : enemies) {
            if (!enemy->GetActive()) {
                continue;
            }

            if (std::abs(enemy->GetPosition().y - bulletY) > enemy->GetCollider()->GetMaxCollisionDistance() + bulletCollisionDistance) {
                continue;
            }

            if (checkCollision(bullet->GetPosition(), enemy->GetPosition(), bullet->GetCollider(), enemy->GetCollider())) {
                enemyAndPlayerBulletsCollisions.emplace_back(CollisionPair(bullet, enemy));
            }
        }
    }

    return enemyAndPlayerBulletsCollisions;
}

const CollisionPair& CollisionManager::EnemyBulletToPlayerCollision() {
    playerAndEnemyBulletCollision.bullet = nullptr;
    playerAndEnemyBulletCollision.object = nullptr;
    for (const std::shared_ptr<Bullet>& bullet : objectManager->GetEnemyBullets()) {
        if (checkCollision(bullet->GetPosition(), objectManager->GetPlayer()->GetPosition(), bullet->GetCollider(), objectManager->GetPlayer()->GetCollider())) {
            playerAndEnemyBulletCollision.bullet = bullet;
            playerAndEnemyBulletCollision.object = objectManager->GetPlayer();
            break;
        }
    }

    return playerAndEnemyBulletCollision;
}