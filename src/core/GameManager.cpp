#include "core/GameManager.h"
#include "scenes/MainScene.h"
#include "glGraphics/Camera.h"
#include "glGraphics/Texture2D.h"
#include "glGraphics/Shader.h"
#include "glGraphics/Model.h"

#include <glad/glad.h>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> 

GameManager::GameManager()
    : sceneManager({{"MainScene", std::make_shared<MainScene>()}}, "MainScene")  {
}

void GameManager::BindCamera(Camera* camera) {
    mainCamera = camera;
}

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

    glClearColor(.07f, .13f, .17f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);

    Shader shaderProgram("src/assets/shaders/default.vert", "src/assets/shaders/default.frag");

    GLuint modelLoc = glGetUniformLocation(shaderProgram.ID, "model");
    GLuint viewLoc = glGetUniformLocation(shaderProgram.ID, "view");
    GLuint projectionLoc = glGetUniformLocation(shaderProgram.ID, "projection");

    Model scene("src/assets/scenes/EOSD_s4.gltf");

    shaderProgram.Activate();
    
    glm::vec3 lightPos(0.f, .75f, 1.5f);
    shaderProgram.SetVec3("lightPos", lightPos);

    glm::vec3 lightColor(1.f, 1.f, 1.f);
    shaderProgram.SetVec3("lightColor", lightColor);

    glm::vec3 cameraPos(0.0, -2.2901699542999268f,  -2.358180046081543f);
    cameraPos = glm::vec3(cameraPos.x, -cameraPos.z, cameraPos.y);
    glm::vec3 cameraTarget = cameraPos + glm::vec3(0.0, -0.6427876949310303f, 0.7660444974899292f);

    Camera camera = Camera(cameraPos, glm::vec3(0.f, 0.f, 0.f));
    camera.LookAt(cameraTarget);

    camera.Aspect = 1.f / aspectRatio;
    BindCamera(&camera);
     
    /*------------------------------------------*/

    float accumulator = 0.f;

    while (window.isOpen()) {
        if (mainCamera == nullptr) {
            throw std::runtime_error("Error: camera object is empty");
        }

        float deltaTime = deltaTimeClock.restart().asSeconds();
        accumulator += deltaTime;

        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>()){
                window.close();
            }
        }

        // CAMERA MOVEMENT FOR DEBUGGING PURPOSE ////////////////////////////

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::W)) {
            mainCamera->SetPosition(mainCamera->GetPosition() + mainCamera->GetDirection() * deltaTime * 10.f);
        } 
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::S)) {
            mainCamera->SetPosition(mainCamera->GetPosition() - mainCamera->GetDirection() * deltaTime * 10.f);
        } 
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::A)) {
            glm::vec3 worldUp = glm::vec3(0.f, 1.f, 0.f);
            glm::vec3 cameraRight = glm::cross(worldUp, mainCamera->GetDirection());
            mainCamera->SetPosition(mainCamera->GetPosition() + cameraRight * deltaTime * 10.f);
        } 
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::D)) {
            glm::vec3 worldUp = glm::vec3(0.f, 1.f, 0.f);
            glm::vec3 cameraRight = glm::cross(worldUp, mainCamera->GetDirection());
            mainCamera->SetPosition(mainCamera->GetPosition() - cameraRight * deltaTime * 10.f);
        } 
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Space)) {
            glm::vec3 worldUp = glm::vec3(0.f, 1.f, 0.f);
            glm::vec3 cameraRight = glm::cross(worldUp, mainCamera->GetDirection());
            glm::vec3 cameraUp = glm::cross(cameraRight, mainCamera->GetDirection());
            mainCamera->SetPosition(mainCamera->GetPosition() - cameraUp * deltaTime * 10.f);
        } 
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::LControl)) {
            glm::vec3 worldUp = glm::vec3(0.f, 1.f, 0.f);
            glm::vec3 cameraRight = glm::cross(worldUp, mainCamera->GetDirection());
            glm::vec3 cameraUp = glm::cross(cameraRight, mainCamera->GetDirection());
            mainCamera->SetPosition(mainCamera->GetPosition() + cameraUp * deltaTime * 10.f);
        } 

        auto rotateCamera = [](float angle, const glm::vec3& axis, Camera*& cam) {
            glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(angle), axis);
            cam->SetDirection(glm::normalize(glm::vec3(rotationMatrix * glm::vec4(cam->GetDirection(), 1.0f))));
        };

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Left)) {
            glm::vec3 worldUp = glm::vec3(0.f, 1.f, 0.f);
            rotateCamera(40.f * deltaTime, worldUp, mainCamera);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Right)) {
            glm::vec3 worldUp = glm::vec3(0.f, 1.f, 0.f);
            rotateCamera(-40.f * deltaTime, worldUp, mainCamera);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Up)) {
            glm::vec3 worldUp = glm::vec3(0.f, 1.f, 0.f);
            glm::vec3 cameraRight = glm::normalize(glm::cross(worldUp, mainCamera->GetDirection()));
            rotateCamera(-40.f * deltaTime, cameraRight, mainCamera); 
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Down)) {
            glm::vec3 worldUp = glm::vec3(0.f, 1.f, 0.f);
            glm::vec3 cameraRight = glm::normalize(glm::cross(worldUp, mainCamera->GetDirection()));
            rotateCamera(40.f * deltaTime, cameraRight, mainCamera);
        }

        /////////////////////////////////////////////////////////////

        update(deltaTime);

        while (accumulator >= fixedDeltaTime) {
            fixedUpdate(fixedDeltaTime);
            accumulator -= fixedDeltaTime;
        }

        window.clear(sf::Color::Black);

        draw();

        /*---------------OpenGL Test----------------*/

        glViewport(32, 16, 384, 448);
        glEnable(GL_SCISSOR_TEST);
        glEnable(GL_DEPTH_TEST);

        glScissor(32, 16, 384, 448);

        glClearColor(.07f, .13f, .17f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shaderProgram.Activate();

        shaderProgram.SetVec3("viewPos", mainCamera->GetPosition());

        glm::mat4 model = glm::mat4(1.f);   

        glm::mat4 view = mainCamera->GetViewMatrix();

        glm::mat4 projection = mainCamera->GetPerspectiveMatrix();

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

        scene.Draw(shaderProgram);

        glViewport(0, 0, windowWidth, windowHeight);
        glDisable(GL_SCISSOR_TEST);
        glDisable(GL_DEPTH_TEST);

        /*------------------------------------------*/

        window.display();
    }

    /*---------------OpenGL Test----------------*/

    shaderProgram.Delete();

    /*------------------------------------------*/
}

void GameManager::fixedUpdate(float fixedDeltaTime) {
    sceneManager.GetCurrentScene()->fixedUpdate(window, fixedDeltaTime);
}

void GameManager::update(float deltaTime) {
    sceneManager.GetCurrentScene()->update(window, deltaTime);
}

void GameManager::draw() {
    sceneManager.GetCurrentScene()->draw(window);
}