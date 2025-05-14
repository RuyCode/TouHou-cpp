#include "core/PlayerMarisa.h"
#include "core/CircleCollider.h"
#include "core/ObjectManager.h"
#include "Bullet.h"


PlayerMarisa::PlayerMarisa() : Player() {

    speed = 300;

    auto defaultTex = std::make_shared<sf::Texture>();
    if (!defaultTex->loadFromFile("src/assets/textures/Player/marisaFumo.png")) {
        throw std::runtime_error("Error: failed to load player texture");
    }

    std::vector<std::shared_ptr<sf::Texture>> textures {
        defaultTex
    };

    Player::textures = textures;
    Player::sprite = std::make_shared<sf::Sprite>(sf::Sprite(*textures[0]));

    Player::collider = std::make_shared<CircleCollider>(CircleCollider(.5f, {0.f, 0.f}));

    attackCooldown = .075f;
    attackTimer = attackCooldown;

    auto attackBulletTexture = std::make_shared<sf::Texture>();

    if (!attackBulletTexture->loadFromFile("src/assets/textures/Player/marisaTempBullet.png")) {
        throw std::runtime_error("Error: failed to load Marisa's bullet texture");
    }

    attackBulletTextures = {
        attackBulletTexture
    };
}

void PlayerMarisa::attack() {
    for (float i = -15.f; i <= 15.f; i += 15.f) {
        if (auto manager = objectManager.lock()){
            std::shared_ptr<Bullet> bullet = manager->GetFreePlayerBullet();
            glm::vec2 pos = position + glm::vec2(sprite->getTexture().getSize().x, sprite->getTexture().getSize().y) * .5f;
            bullet->SetPosition(pos);
            bullet->SetAngle(i - 90);
            bullet->SetSpeed(500.f);
            bullet->SetTextures(attackBulletTextures);
            bullet->SetCollider(std::make_shared<CircleCollider>(CircleCollider(0.5f, {0.f, 0.f})));
        }
    }
}