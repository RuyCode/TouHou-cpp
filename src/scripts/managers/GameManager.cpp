#include "scripts/managers/GameManager.h"
#include "scripts/scenes/EOSD_s4_Scene.h"
#include "scripts/scenes/MainMenu_Scene.h"

#include <glad/glad.h>
#include <SFML/Graphics.hpp>

GameManager::GameManager() {}

void GameManager::Run() {
    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.majorVersion = 4;
    settings.minorVersion = 5;
    settings.attributeFlags = sf::ContextSettings::Default;

    sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();

    window.create(sf::VideoMode({windowWidth, windowHeight}), sf::String(windowName), sf::Style::Titlebar | sf::Style::Close, sf::State::Windowed, settings);
    window.setPosition({static_cast<int>(desktopMode.size.x / 2u - windowWidth / 2u), static_cast<int>(desktopMode.size.y / 2u - windowHeight / 2u)});

    window.setFramerateLimit(fps);
    window.setVerticalSyncEnabled(false);

    gladLoadGL();

    sceneManager = SceneManager(
        {
            {"MainMenu", std::make_shared<MainMenu_Scene>(window)},
            {"EOSD_s4", std::make_shared<EOSD_s4_Scene>(window)},
        },
         "MainMenu"
    );

    float accumulator = 0.f;

    while (window.isOpen()) {
        float deltaTime = deltaTimeClock.restart().asSeconds();
        accumulator += deltaTime;

        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>()){
                window.close();
            }
        }

        if (!window.isOpen()) break;

        while (accumulator >= fixedDeltaTime) {
            fixedUpdate(fixedDeltaTime);
            accumulator -= fixedDeltaTime;
        }

        update(deltaTime);
        draw();

        window.display();
    }
}

void GameManager::fixedUpdate(float fixedDeltaTime) {
    auto currentScene = sceneManager.GetCurrentScene();
    currentScene->FixedUpdate(window, fixedDeltaTime);
}

void GameManager::update(float deltaTime) {
    auto currentScene = sceneManager.GetCurrentScene();
    currentScene->Update(window, deltaTime);

    if (!currentScene->GetNextSceneName().empty()) {
        std::string nextSceneName = currentScene->GetNextSceneName();
        currentScene->ResetNextSceneName();
        sceneManager.LoadScene(nextSceneName);
    }
}

void GameManager::draw() {
    window.clear(sf::Color::Black);
    
    sceneManager.GetCurrentScene()->Draw(window);
}