#include "core/Enemy.h"
#include "core/CircleCollider.h"

Enemy::Enemy() : GameObject({0.f, 0.f}, std::make_shared<CircleCollider>(CircleCollider(0.f, {0.f, 0.f}))) {

}

void Enemy::FixedUpdate(float fixedDeltaTime) {

}

void Enemy::Update(float deltaTime) {

}
    
void Enemy::Draw(sf::RenderTarget& target) {

}