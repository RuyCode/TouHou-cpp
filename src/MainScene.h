#include <SFML/Graphics.hpp>

#include "Scene.h"

#ifndef MAIN_SCENE_H
#define MAIN_SCENE_H

class MainScene : public Scene {
public:
    void FixedUpdate(sf::RenderWindow& window, float fixedDeltaTime) override;

    void Update(sf::RenderWindow& window, float deltaTime) override;

    void Draw(sf::RenderWindow& window) override;
};

#endif // MAIN_SCENE_H