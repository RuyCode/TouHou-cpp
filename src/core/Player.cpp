#include "core/Player.h"
#include "CircleCollider.h"

#include <memory>
#include <SFML/Graphics.hpp>
#include <glm/glm.hpp>

Player::Player() : GameObject({0.f, 0.f}, std::make_shared<CircleCollider>(CircleCollider(0.f, {0.f, 0.f}))) {}

Player::Player(std::vector<std::shared_ptr<sf::Texture>> textures, float speed) 
  : textures(textures), 
    speed(speed), 
    sprite(std::make_shared<sf::Sprite>(sf::Sprite(*textures[0]))),
    GameObject({194.f, 224.f}, std::make_shared<CircleCollider>(CircleCollider(0.5f, {0.f, 0.f}))){
}

Player::Player(const Player& other) 
  : GameObject(other.position, other.collider), 
    textures(other.textures), 
    speed(other.speed) {
    
    if (other.sprite) {
        sprite = std::make_shared<sf::Sprite>(*other.sprite);
    }
}

Player& Player::operator=(const Player& other) {
    if (*this == other) {
        return *this;
    }
    
    textures = other.textures;
    speed = other.speed;
    sprite = other.sprite;

    position = other.position;
    collider = other.collider;

    return *this;
}

void Player::FixedUpdate(float fixedDeltaTime) {

} 

void Player::Update(float deltaTime) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Left)) {
        position += glm::vec2(-1.f, 0.f) * speed * deltaTime;
    } 

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Right)) {
        position += glm::vec2(1.f, 0.f) * speed * deltaTime;
    } 

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Up)) {
        position += glm::vec2(0.f, -1.f) * speed * deltaTime;
    } 

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Down)) {
        position += glm::vec2(0.f, 1.f) * speed * deltaTime;
    } 

    if (position.x - sprite->getTexture().getSize().x * .5f < 0.f) {
        position.x = sprite->getTexture().getSize().x * .5f;
    }

    if (position.y - sprite->getTexture().getSize().y * .5f < 0.f) {
        position.y = sprite->getTexture().getSize().y * .5f;
    }

    if (position.x + sprite->getTexture().getSize().x * .5f > 384.f) {
        position.x = 384.f - sprite->getTexture().getSize().x * .5f;
    }

    if (position.y + sprite->getTexture().getSize().y * .5f> 448.f) {
        position.y = 448.f - sprite->getTexture().getSize().y * .5f;
    }

    sprite->setPosition(sf::Vector2f(position.x, position.y));

    attackTimer -= deltaTime;
    if (attackTimer <= 0) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Z)) {
            attack();
        }
        attackTimer = attackCooldown;
    }
}

void Player::Draw(sf::RenderTarget& target) {
    target.draw(*sprite);
}

void Player::SetObjectManager(std::shared_ptr<ObjectManager> objectManager) {
    this->objectManager = objectManager;
}