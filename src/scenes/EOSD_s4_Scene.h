#include "backgrounds/EOSD_s4_Background.h"
#include "core/StageScene.h"
#include "core/ObjectManager.h"
#include "core/CollisionManager.h"

#ifndef MAIN_SCENE_H
#define MAIN_SCENE_H

#include <SFML/Graphics.hpp>

class EOSD_s4_Scene : public StageScene {
private:

public:
    EOSD_s4_Scene(sf::Window& window);

    void FixedUpdate(sf::RenderWindow& window, float fixedDeltaTime) override;

    void Update(sf::RenderWindow& window, float deltaTime) override;

    void Draw(sf::RenderWindow& window) override;
};

#endif // MAIN_SCENE_H
