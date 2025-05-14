#include "core/GameObject.h"
#include "core/ObjectManager.h"

#ifndef PLAYER_H
#define PLAYER_H

#include <memory>
#include <vector>
#include <SFML/Graphics.hpp>
#include <glm/glm.hpp>

class Player : public GameObject {
protected:
    std::weak_ptr<ObjectManager> objectManager;
    std::vector<std::shared_ptr<sf::Texture>> textures;
    std::shared_ptr<sf::Sprite> sprite;

    std::vector<std::shared_ptr<sf::Texture>> attackBulletTextures;

    float attackCooldown;
    float attackTimer;

    float speed;

    virtual void attack() = 0;

public: 
    Player();

    Player(std::vector<std::shared_ptr<sf::Texture>> textures, float speed);

    Player(const Player& other);

    Player& operator=(const Player& other);

    void SetObjectManager(std::shared_ptr<ObjectManager> objectManager);

    void FixedUpdate(float fixedDeltaTime) override;    

    void Update(float deltaTime) override;
    
    void Draw(sf::RenderTarget& target) override;
};

#endif