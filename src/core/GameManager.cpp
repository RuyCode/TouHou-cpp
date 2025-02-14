#include "core/GameManager.h"
#include "scenes/MainScene.h"
#include "graphics/Shader.h"
#include "graphics/VertexBufferObject.h"
#include "graphics/ElementBufferObject.h"
#include "graphics/VertexArrayObject.h"

#include <glad/glad.h>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <cmath>

GameManager::GameManager()
    : sceneManager({{"MainScene", std::make_shared<MainScene>()}}, "MainScene")  {
}

void GameManager::Run() {
    window.create(sf::VideoMode({windowWidth, windowHeight}), "Touhou c++");
    window.setFramerateLimit(fps);

    /*---------------OpenGL Test----------------*/

    gladLoadGL();

    const float aspectRatio = 448.f / 384.f;
    
    GLfloat vertices[] = {
        -0.5f * aspectRatio, -0.5f * static_cast<float>(sqrt(3)) / 3.f, 0.f, // Lower left corner
        0.5f * aspectRatio, -0.5f * static_cast<float>(sqrt(3)) / 3.f, 0.f,  // Lower right corner    
        0.f * aspectRatio, 0.5f * static_cast<float>(sqrt(3)) * 2.f / 3.f, 0.f, // Upper corner
        -0.5f / 2.f * aspectRatio, 0.5f * float(sqrt(3)) / 6.f, 0.f, // Inner left
        0.5f / 2.f * aspectRatio, 0.5f * float(sqrt(3)) / 6.f, 0.f, // Inner right
        0.f * aspectRatio, -0.5f * float(sqrt(3)) / 3.f, 0.f // Inner down
    };

    GLuint indices[] = {
        0, 3, 5, // Lower left triangle
        3, 2, 4, // Lower right triangle
        5, 4, 1, // Upper triangle
    };


    glClearColor(.07f, .13f, .17f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);

    Shader shaderProgram("src/assets/shaders/default.vert", "src/assets/shaders/default.frag");

    VertexArrayObject VAO;
    VAO.Bind();

    VertexBufferObject VBO(vertices, sizeof(vertices));
    ElementBufferObject EBO(indices, sizeof(indices));

    VAO.LinkVBO(VBO, 0);

    VAO.Unbind();
    VBO.Unbind();
    EBO.Unbind();
    
    /*------------------------------------------*/

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

        /*---------------OpenGL Test----------------*/

        glViewport(32, 16, 384, 448);
        glEnable(GL_SCISSOR_TEST);
        glScissor(32, 16, 384, 448);

        glClearColor(.07f, .13f, .17f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        shaderProgram.Activate();

        VAO.Bind();
        glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);

        glViewport(0, 0, windowWidth, windowHeight);
        glDisable(GL_SCISSOR_TEST);

        /*------------------------------------------*/

        window.display();
    }

    /*---------------OpenGL Test----------------*/

    VAO.Delete();
    VBO.Delete();
    EBO.Delete();
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