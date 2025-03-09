#include "scenes/Scene.h"
#include "glGraphics/EOSD_s4_Background.h"

#ifndef MAIN_SCENE_H
#define MAIN_SCENE_H

#include <SFML/Graphics.hpp>

class EOSD_s4_Scene : public Scene {
private:
    EOSD_s4_Background background;
public:
    EOSD_s4_Scene();

    void FixedUpdate(sf::RenderWindow& window, float fixedDeltaTime) override;

    void Update(sf::RenderWindow& window, float deltaTime) override;

    void Draw(sf::RenderWindow& window) override;
};

#endif // MAIN_SCENE_H
