#include "core/GameManager.h"
#include "scenes/EOSD_s4_Scene.h"
#include "scenes/MainMenu_Scene.h"

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
    window.setPosition({desktopMode.size.x / 2 - windowWidth / 2, desktopMode.size.y / 2 - windowHeight / 2});

    window.setFramerateLimit(fps);

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

        update(deltaTime);

        while (accumulator >= fixedDeltaTime) {
            fixedUpdate(fixedDeltaTime);
            accumulator -= fixedDeltaTime;
        }

        draw();

        window.display();
    }
}

void GameManager::fixedUpdate(float fixedDeltaTime) {
    sceneManager.GetCurrentScene()->FixedUpdate(window, fixedDeltaTime);
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