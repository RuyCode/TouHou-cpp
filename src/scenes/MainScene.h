#include "scenes/Scene.h"

#ifndef MAIN_SCENE_H
#define MAIN_SCENE_H

#include <SFML/Graphics.hpp>

class MainScene : public Scene {
public:
    void fixedUpdate(sf::RenderWindow& window, float fixedDeltaTime) override;

    void update(sf::RenderWindow& window, float deltaTime) override;

    void draw(sf::RenderWindow& window) override;
};

#endif // MAIN_SCENE_H