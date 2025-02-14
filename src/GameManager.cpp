#include "GameManager.h"
#include "MainScene.h"

#include <SFML/Graphics.hpp>


GameManager::GameManager() 
    : sceneManager({{"MainScene", std::make_shared<MainScene>()}}, "MainScene") {
}

void GameManager::Run() {
    window.create(sf::VideoMode({windowWidth, windowHeight}), "");
    window.setFramerateLimit(fps);

    float accumulator = 0.f;

    while (window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();
        accumulator += deltaTime;

        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>()){
                window.close();
            }
        }

        Update(deltaTime);

        while (accumulator >= fixedDeltaTime) {
            FixedUpdate(fixedDeltaTime);
            accumulator -= fixedDeltaTime;
        }

        window.clear(sf::Color::Black);
        Draw();
        window.display();
    }
}

void GameManager::FixedUpdate(float fixedDeltaTime) {
    sceneManager.GetCurrentScene()->FixedUpdate(window, fixedDeltaTime);
}

void GameManager::Update(float deltaTime) {
    sceneManager.GetCurrentScene()->Update(window, deltaTime);
}

void GameManager::Draw() {
    sceneManager.GetCurrentScene()->Draw(window);
}