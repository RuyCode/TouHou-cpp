#include "scripts/entities/GameObject.h"
#include "scripts/managers/ObjectManager.h"
#include "scripts/utils/Spline.h"

#ifndef ENEMY_H
#define ENEMY_H

class Enemy : public GameObject {
private:
    float health;

    Spline path;
    float travelTime;

    float t;

protected: 
    std::vector<std::shared_ptr<sf::Texture>> textures;
    std::shared_ptr<sf::Sprite> sprite;

public:
    Enemy();

    void SetHealth(float health);

    float GetHealth();

    void SetPath(const Spline& path);

    void SetTextures(const std::vector<std::shared_ptr<sf::Texture>>& textures);

    void SetTravelTime(float travelTime);

    void FixedUpdate(float fixedDeltaTime) override; 

    void Update(float deltaTime) override;
    
    void Draw(sf::RenderTarget& target) override;

};

#endif // ENEMY_H
