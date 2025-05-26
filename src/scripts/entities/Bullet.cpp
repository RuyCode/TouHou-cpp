#include "scripts/entities/Bullet.h"
#include "scripts/colliders/CircleCollider.h"
#include "scripts/utils/Debug.h"

Bullet::Bullet() : GameObject({0.f, 0.f}, std::make_shared<CircleCollider>(CircleCollider(0.f, {0.f, 0.f})))  {
    velocity = speed * glm::vec2(glm::cos(glm::radians(angle)), glm::sin(glm::radians(angle)));
}

float Bullet::GetSpeed() {
    return speed;
} 

float Bullet::GetAngle() {
    return angle;
}

void Bullet::SetSpeed(float value) {
    velocity /= speed;
    speed = value;
    velocity *= speed;
}

void Bullet::SetAngle(float value) {
    angle = value;
    velocity = speed * glm::vec2(glm::cos(glm::radians(angle)), glm::sin(glm::radians(angle)));
}

void Bullet::SetTextures(const std::vector<std::shared_ptr<sf::Texture>>& textures) {
    this->textures = textures;

    if (!sprite) {
        sprite = std::make_shared<sf::Sprite>(*textures[0]);
    }

    sprite->setTexture(*textures[0]); 
}

void Bullet::FixedUpdate(float fixedDeltaTime) {

}

void Bullet::Update(float deltaTime) {
    position += velocity * deltaTime;

    if (position.x < -50 || position.x > 434 || position.y < -50 || position.y > 498) {
        SetActive(false);
    }
}
    
void Bullet::Draw(sf::RenderTarget& target) {
    if (!sprite) return;
    sprite->setOrigin({sprite->getLocalBounds().size.x / 2, sprite->getLocalBounds().size.y / 2});
    sprite->setPosition({position.x, position.y});
    sf::Angle rad = sf::degrees(90 + angle);
    sprite->setRotation(rad);
    target.draw(*sprite);

    if (DEBUG_MODE) {
        DrawHitbox(target, sf::Color::Green);
    }
}