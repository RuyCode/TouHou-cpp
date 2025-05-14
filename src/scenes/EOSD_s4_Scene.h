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

    void FixedUpdate(sf::Window& window, float fixedDeltaTime) override;

    void Update(sf::Window& window, float deltaTime) override;

    void Draw(sf::RenderTarget& target) override;
};

#endif // MAIN_SCENE_H
