#include "scripts/entities/Enemy.h"
#include "scripts/colliders/CircleCollider.h"
#include "scripts/utils/Spline.h"
#include "scripts/utils/Env.h"
#include "scripts/utils/Debug.h"

Enemy::Enemy()
  : GameObject({0.f, 0.f}, std::make_shared<CircleCollider>(CircleCollider(12.f, {0.f, 0.f}))),
    path(Spline({}, glm::mat4(), 0)),
    t(0.f) {
}

void Enemy::SetHealth(float health) {
    this->health = health;
}

float Enemy::GetHealth() {
    return health;
};

void Enemy::SetPath(const Spline& path) {
    this->path = path;
    
    position = this->path.GetUniformPoint(0.f);
}

void Enemy::SetTravelTime(float travelTime) {
    this->travelTime = travelTime;
}

void Enemy::SetTextures(const std::vector<std::shared_ptr<sf::Texture>>& textures) {
    this->textures = textures;

    if (!sprite) {
        sprite = std::make_shared<sf::Sprite>(*textures[0]);
    }

    sprite->setTexture(*textures[0]); 
}

void Enemy::FixedUpdate(float fixedDeltaTime) {

}

void Enemy::Update(float deltaTime) {
    t += deltaTime / travelTime;
    position = path.GetUniformPoint(t);
    position.y = Env::gameViewportHeight - position.y;

    if (health <= 0) {
        SetActive(false);
    }

    if (t >= 1.f) {
        SetActive(false);
    }
}
    
void Enemy::Draw(sf::RenderTarget& target) {
    if (!sprite) return;
    sprite->setOrigin({sprite->getLocalBounds().size.x / 2, sprite->getLocalBounds().size.y / 2});
    sprite->setPosition({position.x, position.y});
    target.draw(*sprite);

    if (DEBUG_MODE) {
        DrawHitbox(target, sf::Color::Red);
    }
}