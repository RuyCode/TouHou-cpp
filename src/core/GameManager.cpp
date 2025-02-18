#include "core/GameManager.h"
#include "scenes/MainScene.h"
#include "graphics/Texture2D.h"
#include "graphics/Shader.h"
#include "graphics/VertexBufferObject.h"
#include "graphics/ElementBufferObject.h"
#include "graphics/VertexArrayObject.h"

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

    GLfloat vertices[] = {
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f
    };

    glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f, 0.0f, 0.0f),
        glm::vec3( 2.0f, 5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f, 3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f, 2.0f, -2.5f),
        glm::vec3( 1.5f, 0.2f, -1.5f),
        glm::vec3(-1.3f, 1.0f, -1.5f)
    };

    glClearColor(.07f, .13f, .17f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);

    Shader shaderProgram("src/assets/shaders/default.vert", "src/assets/shaders/default.frag");

    VertexArrayObject VAO;
    VAO.Bind();

    VertexBufferObject VBO(vertices, sizeof(vertices));

    VAO.LinkAttrib(VBO, 0, 3, GL_FLOAT, 5 * sizeof(float), reinterpret_cast<void*>(0));
    VAO.LinkAttrib(VBO, 1, 2, GL_FLOAT, 5 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));

    VAO.Unbind();
    VBO.Unbind();

    Texture2D texture0("src/assets/textures/pixel-art-character-niko-one-shot-game-s2pacfuyat98l4v3.jpg");
    Texture2D texture1("src/assets/textures/7561297a-6cb7-46a1-9b7f-6401274c4f02.jpg");

    GLuint tex0uniID = glGetUniformLocation(shaderProgram.ID, "tex0");
    GLuint tex1uniID = glGetUniformLocation(shaderProgram.ID, "tex1");
    
    GLuint modelLoc = glGetUniformLocation(shaderProgram.ID, "model");
    GLuint viewLoc = glGetUniformLocation(shaderProgram.ID, "view");
    GLuint projectionLoc = glGetUniformLocation(shaderProgram.ID, "projection");

    shaderProgram.Activate();
    glUniform1i(tex0uniID, 0);
    glUniform1i(tex0uniID, 1);
     
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

        Update(deltaTime);

        while (accumulator >= fixedDeltaTime) {
            FixedUpdate(fixedDeltaTime);
            accumulator -= fixedDeltaTime;
        }

        window.clear(sf::Color::Black);

        Draw();

        /*---------------OpenGL Test----------------*/

        glViewport(32, 16, 384, 448);
        glEnable(GL_SCISSOR_TEST);
        glEnable(GL_DEPTH_TEST);

        glScissor(32, 16, 384, 448);

        glClearColor(.07f, .13f, .17f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shaderProgram.Activate();

        texture0.BindToUnit(0);
        texture1.BindToUnit(1);

        VAO.Bind();

        for(uint i = 0; i < 10; i++){
            glm::mat4 model = glm::mat4(1.f);   
            model = glm::translate(model, cubePositions[i]);
            model = glm::rotate(model, glm::radians(-55.f) * clock.getElapsedTime().asSeconds(), glm::vec3(.5f, 1.f, 0.f));

            glm::mat4 view = glm::mat4(1.f);
            view = glm::translate(view, glm::vec3(0.f, 0.f, -3.f));

            glm::mat4 projection = glm::mat4(1.f);
            projection = glm::perspective(glm::radians(45.f), 1.f / aspectRatio, .1f, 100.f);

            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
            glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }


        glViewport(0, 0, windowWidth, windowHeight);
        glDisable(GL_SCISSOR_TEST);
        glDisable(GL_DEPTH_TEST);

        /*------------------------------------------*/

        window.display();
    }

    /*---------------OpenGL Test----------------*/

    VAO.Delete();
    VBO.Delete();
    shaderProgram.Delete();

    /*------------------------------------------*/
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