#include <SFML/Graphics.hpp>

#include "SceneManager.h"

#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

class GameManager {
private:
    sf::RenderWindow window;
    const char* windowName = "Touhou c++";

    sf::Clock clock;
    const uint8_t fps = 60u;
    const float fixedDeltaTime = 1. / static_cast<float>(fps);

    const uint16_t windowWidth = 640u;
    const uint16_t windowHeight = 480u;

    SceneManager sceneManager;

    void FixedUpdate(float fixedDeltaTime);

    void Update(float deltaTime);

    void Draw();

public:
    GameManager();

    void Run();
};

#endif // GAME_MANAGER_Hs