#include "core/GameObject.h"
#include "core/ObjectManager.h"

#ifndef ENEMY_H
#define ENEMY_H

class Enemy : public GameObject {
public:
    Enemy();

    void FixedUpdate(float fixedDeltaTime) override;    

    void Update(float deltaTime) override;
    
    void Draw(sf::RenderTarget& target) override;

};

#endif // ENEMY_H
