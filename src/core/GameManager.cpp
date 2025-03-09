#include "core/GameManager.h"
#include "scenes/EOSD_s4_Scene.h"
#include "glGraphics/model/Model.h"

#include <glad/glad.h>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> 

GameManager::GameManager() {}

void GameManager::Run() {
    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.majorVersion = 4;
    settings.minorVersion = 5;
    settings.attributeFlags = sf::ContextSettings::Default;

    window.create(sf::VideoMode({windowWidth, windowHeight}), sf::String(windowName), sf::State::Windowed, settings);
    window.setFramerateLimit(fps);

    /*---------------OpenGL Test----------------*/

    gladLoadGL();

    const float aspectRatio = 448.f / 384.f;

    glClear(GL_COLOR_BUFFER_BIT);

    sceneManager = SceneManager({{"EOSD_s4", std::make_shared<EOSD_s4_Scene>()}}, "EOSD_s4");


    /*------------------------------------------*/

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

        update(deltaTime);

        while (accumulator >= fixedDeltaTime) {
            fixedUpdate(fixedDeltaTime);
            accumulator -= fixedDeltaTime;
        }

        window.clear(sf::Color::Black);

        /*---------------OpenGL Test----------------*/

        glViewport(32, 16, 384, 448);
        glEnable(GL_SCISSOR_TEST);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


        glScissor(32, 16, 384, 448);

        glm::vec4 clearColor(1.f, 1.f, 1.f, 1.f);

        glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        draw();

        glDisable(GL_SCISSOR_TEST);

        /*------------------------------------------*/

        window.display();
    }
}

void GameManager::fixedUpdate(float fixedDeltaTime) {
    sceneManager.GetCurrentScene()->FixedUpdate(window, fixedDeltaTime);
}

void GameManager::update(float deltaTime) {
    sceneManager.GetCurrentScene()->Update(window, deltaTime);
}

void GameManager::draw() {
    sceneManager.GetCurrentScene()->Draw(window);
}