#include "core/GameObject.h"

#ifndef ENEMY_H
#define ENEMY_H

class Enemy : public GameObject {
public:
    Enemy();

    void FixedUpdate(float fixedDeltaTime) override;    

    void Update(float deltaTime) override;
    
    void Draw(sf::RenderWindow& window) override;

};

#endif // ENEMY_H
