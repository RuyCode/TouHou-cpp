#include "scripts/entities/GameObject.h"
#include "scripts/managers/ObjectManager.h"

#ifndef BULLET_H
#define BULLET_H

#include <iostream>

class Bullet : public GameObject {
private:
    glm::vec2 velocity;

    float speed;
    float angle;
protected:
    std::vector<std::shared_ptr<sf::Texture>> textures;
    std::shared_ptr<sf::Sprite> sprite;

    bool isHoming;
public:
    Bullet();
    
    float GetSpeed();

    float GetAngle();

    void SetSpeed(float value);
    
    void SetAngle(float value);

    void SetTextures(const std::vector<std::shared_ptr<sf::Texture>>& textures);

    void FixedUpdate(float fixedDeltaTime) override;    

    void Update(float deltaTime) override;
    
    void Draw(sf::RenderTarget& target) override;
};

#endif // BULLET_H
