#include "scripts/backgrounds/EOSD_s4_Background.h"
#include "scripts/scenes/StageScene.h"
#include "scripts/managers/ObjectManager.h"
#include "scripts/managers/CollisionManager.h"

#ifndef MAIN_SCENE_H
#define MAIN_SCENE_H

#include <SFML/Graphics.hpp>

class EOSD_s4_Scene : public StageScene {
private:

public:
    EOSD_s4_Scene(sf::RenderWindow& window);

    void FixedUpdate(sf::RenderWindow& window, float fixedDeltaTime) override;

    void Update(sf::RenderWindow& window, float deltaTime) override;

    void Draw(sf::RenderTarget& target) override;
};

#endif // MAIN_SCENE_H
