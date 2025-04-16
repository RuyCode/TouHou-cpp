#include "core/SceneManager.h"
#include "core/Player.h"

#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include <SFML/Graphics.hpp>
#include <memory>

class GameManager {
private:
    sf::RenderWindow window;
    const char* windowName = "Touhou c++";

    sf::Clock deltaTimeClock;
    sf::Clock clock;
    const uint8_t fps = 60u;
    const float fixedDeltaTime = 1.f / static_cast<float>(fps);

    const uint16_t windowWidth = 640u;
    const uint16_t windowHeight = 480u;

    SceneManager sceneManager;

    void initializeOpenGL();

    void fixedUpdate(float fixedDeltaTime);

    void update(float deltaTime);

    void draw();

public:
    GameManager();

    void Run();
};

#endif // GAME_MANAGER_H