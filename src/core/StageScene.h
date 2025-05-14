#include "core/Scene.h"
#include "scenes/Pause_Scene.h"
#include "core/ObjectManager.h"
#include "core/CollisionManager.h"
#include "glGraphics/GLBackground.h"

#ifndef STAGE_SCENE_H
#define STAGE_SCENE_H

class StageScene : public Scene {
protected:
    sf::Window& window;

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
    std::uint16_t power;

    std::shared_ptr<ObjectManager> objectManager;
    CollisionManager collisionManager;
public:
    StageScene(sf::Window& window, std::shared_ptr<GLBackground> background);

    void FixedUpdate(sf::Window& window, float fixedDeltaTime) override;

    void Update(sf::Window& window, float deltaTime) override;

    void Draw(sf::RenderTarget& target) override;
};

#endif // STAGE_SCENE_H