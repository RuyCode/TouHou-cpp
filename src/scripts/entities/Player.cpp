#include "scripts/entities/Player.h"
#include "scripts/colliders/CircleCollider.h"
#include "scripts/utils/Env.h"
#include "scripts/utils/Debug.h"

#include <memory>
#include <SFML/Graphics.hpp>
#include <glm/glm.hpp>

Player::Player() : GameObject({0.f, 0.f}, std::make_shared<CircleCollider>(CircleCollider(0.f, {0.f, 0.f}))) {}

Player::Player(std::vector<std::shared_ptr<sf::Texture>> textures, float speed) 
  : textures(textures), 
    baseSpeed(speed), 
    sprite(std::make_shared<sf::Sprite>(sf::Sprite(*textures[0]))),
    GameObject({194.f, 224.f}, std::make_shared<CircleCollider>(CircleCollider(0.f, {0.f, 0.f}))){
}

Player::Player(const Player& other) 
  : GameObject(other.position, other.collider), 
    power(other.power),
    textures(other.textures), 
    baseSpeed(other.baseSpeed) {
    
    if (other.sprite) {
        sprite = std::make_shared<sf::Sprite>(*other.sprite);
    }
}

Player& Player::operator=(const Player& other) {
    if (*this == other) {
        return *this;
    }
    
    textures = other.textures;
    baseSpeed = other.baseSpeed;
    sprite = other.sprite;

    position = other.position;
    collider = other.collider;

    return *this;
}

float Player::GetPower() {
    return power;
}

void Player::SetPower(float power) {
    this->power = power;
}

void Player::FixedUpdate(float fixedDeltaTime) {

} 

void Player::Update(float deltaTime) {

    float speed = baseSpeed;
    direction = glm::vec2(0.f, 0.f);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Left)) {
        direction.x -= 1.f;
    } 

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Right)) {
        direction.x += 1.f;
    } 

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Up)) {
        direction.y -= 1.f;
    } 

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Down)) {
        direction.y += 1.f;
    } 

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::LShift)) {
        speed /= 2;
    } 

    /////////////DEBUG CODE ONLY DELETE LATER//////////////

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::P)) {
        power = std::min(power + 1, 400);
    } 
    ///////////////////////////////////////////////////////

    position += (glm::length(direction) > 0.0001f ? glm::normalize(direction) : glm::vec2(0.f, 0.f)) * speed * deltaTime;

    if (position.x - sprite->getTexture().getSize().x * .5f < 0.f) {
        position.x = sprite->getTexture().getSize().x * .5f;
    }

    if (position.y - sprite->getTexture().getSize().y * .5f < 0.f) {
        position.y = sprite->getTexture().getSize().y * .5f;
    }

    if (position.x + sprite->getTexture().getSize().x * .5f > static_cast<float>(Env::gameViewportWidth)) {
        position.x = static_cast<float>(Env::gameViewportWidth) - sprite->getTexture().getSize().x * .5f;
    }

    if (position.y + sprite->getTexture().getSize().y * .5f > static_cast<float>(Env::gameViewportHeight)) {
        position.y = static_cast<float>(Env::gameViewportHeight) - sprite->getTexture().getSize().y * .5f;
    }

    attackTimer -= deltaTime;
    if (attackTimer <= 0) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Z)) {
            attack();
        }
        attackTimer = attackCooldown;
    }
}

void Player::Draw(sf::RenderTarget& target) {
    sprite->setOrigin({sprite->getLocalBounds().size.x / 2, sprite->getLocalBounds().size.y / 2});
    sprite->setPosition({position.x, position.y});

    sf::Angle rad = sf::degrees(direction.x * 5.f);
    sprite->setRotation(rad);

    target.draw(*sprite);
    
    if (DEBUG_MODE) {
        DrawHitbox(target, sf::Color::Blue);
    }
}

void Player::SetObjectManager(std::shared_ptr<ObjectManager> objectManager) {
    this->objectManager = objectManager;
}