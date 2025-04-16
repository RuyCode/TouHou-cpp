#include "core/GameObject.h"

#ifndef BULLET_H
#define BULLET_H

class Bullet : public GameObject {
public:
    Bullet();

    void FixedUpdate(float fixedDeltaTime) override;    

    void Update(float deltaTime) override;
    
    void Draw(sf::RenderWindow& window) override;
};

#endif // BULLET_H
