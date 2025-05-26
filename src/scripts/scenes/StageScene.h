#include "scripts/scenes/Scene.h"
#include "scripts/scenes/Pause_Scene.h"
#include "scripts/managers/ObjectManager.h"
#include "scripts/managers/CollisionManager.h"
#include "scripts/glGraphics/GLBackground.h"

#ifndef STAGE_SCENE_H
#define STAGE_SCENE_H

class StageScene : public Scene {
protected:
    sf::RenderWindow& window;

    float time = 0.f;

    bool isPaused = false;
    Pause_Scene pauseScene;


    std::shared_ptr<GLBackground> background;

    sf::Texture uiBackgroundTexture;
    sf::Sprite uiBackgroundSprite;

    sf::Font statFont;
    sf::Text statText;

    float currentFPS;

    std::uint32_t hiScore;
    std::uint32_t score;

    std::uint8_t player;

    std::shared_ptr<ObjectManager> objectManager;
    std::shared_ptr<CollisionManager> collisionManager;
public:
    StageScene(sf::RenderWindow& window, std::shared_ptr<GLBackground> background);

    void FixedUpdate(sf::RenderWindow& window, float fixedDeltaTime) override;

    void Update(sf::RenderWindow& window, float deltaTime) override;

    void Draw(sf::RenderTarget& target) override;
};

#endif // STAGE_SCENE_H