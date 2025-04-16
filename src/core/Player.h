#include "core/GameObject.h"

#ifndef PLAYER_H
#define PLAYER_H

#include <memory>
#include <vector>
#include <SFML/Graphics.hpp>
#include <glm/glm.hpp>

class Player : public GameObject {
private:
    std::vector<std::shared_ptr<sf::Texture>> textures;
    std::shared_ptr<sf::Sprite> sprite;

    float speed;

public: 
    Player();

    Player(std::vector<std::shared_ptr<sf::Texture>> textures, float speed);

    Player(const Player& other);

    Player& operator=(const Player& other);

    void FixedUpdate(float fixedDeltaTime) override;    

    void Update(float deltaTime) override;
    
    void Draw(sf::RenderWindow& window) override;
};

#endif