#include "core/Bullet.h"
#include "core/CircleCollider.h"

Bullet::Bullet() : GameObject({0.f, 0.f}, std::make_shared<CircleCollider>(CircleCollider(0.f, {0.f, 0.f})))  {

}

void Bullet::FixedUpdate(float fixedDeltaTime) {

}

void Bullet::Update(float deltaTime) {

}
    
void Bullet::Draw(sf::RenderWindow& window) {

}