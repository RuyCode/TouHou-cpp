#ifndef SCENE_H
#define SCENE_H

#include <SFML/Graphics.hpp>

class Scene {
public:
    virtual void fixedUpdate(sf::RenderWindow& window, float fixedDeltaTime) = 0;

    virtual void update(sf::RenderWindow& window, float deltaTime) = 0;

    virtual void draw(sf::RenderWindow& window) = 0;
};

#endif // SCENE_H