#include <SFML/Graphics.hpp>

#ifndef SCENE_H
#define SCENE_H

class Scene {
public:
    virtual ~Scene() = default;

    virtual void FixedUpdate(sf::RenderWindow& window, float fixedDeltaTime) = 0;

    virtual void Update(sf::RenderWindow& window, float deltaTime) = 0;

    virtual void Draw(sf::RenderWindow& window) = 0;
};

#endif // SCENE_H