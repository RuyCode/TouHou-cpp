#include "scripts/entities/PlayerMarisa.h"
#include "scripts/colliders/CircleCollider.h"
#include "scripts/managers/ObjectManager.h"
#include "scripts/entities/Bullet.h"


PlayerMarisa::PlayerMarisa() : Player() {

    baseSpeed = 300;

    auto defaultTex = std::make_shared<sf::Texture>();
    if (!defaultTex->loadFromFile("src/assets/textures/players/marisaIdle.png")) {
        throw std::runtime_error("Error: failed to load player texture");
    }

    std::vector<std::shared_ptr<sf::Texture>> textures {
        defaultTex
    };

    Player::textures = textures;
    Player::sprite = std::make_shared<sf::Sprite>(sf::Sprite(*textures[0]));

    Player::collider = std::make_shared<CircleCollider>(CircleCollider(3.f, {0.f, 0.f}));

    attackCooldown = 1.f / 12.f;
    attackTimer = attackCooldown;

    auto attackBulletTexture = std::make_shared<sf::Texture>();

    if (!attackBulletTexture->loadFromFile("src/assets/textures/bullets/marisaBullet.png")) {
        throw std::runtime_error("Error: failed to load Marisa's bullet texture");
    }

    attackBulletTextures = {
        attackBulletTexture
    };
}

void PlayerMarisa::attack() {
    std::uint8_t powerLevel = power / 100;

    for (float i = -2.5 * powerLevel; i <= 2.5 * powerLevel; i += 5.f) {
        if (auto manager = objectManager.lock()){
            std::shared_ptr<Bullet> bullet = manager->GetFreePlayerBullet();
            glm::vec2 pos = position + glm::vec2(0, -10);
            bullet->SetPosition(pos);
            bullet->SetAngle(i - 90);
            bullet->SetSpeed(720.f);
            bullet->SetTextures(attackBulletTextures);
            bullet->SetCollider(std::make_shared<CircleCollider>(CircleCollider(5.f, {0.f, -10.f})));
        }
    }
}