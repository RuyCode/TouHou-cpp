#include "glGraphics/GLBackground.h"

#ifndef SCENE_H
#define SCENE_H

#include <SFML/Graphics.hpp>
#include <functional>

class Scene {
protected:
    GLint maxTextureUnits;
    std::vector<GLint> boundTexturesBuffer;
    GLint activeTextureBuffer;

    std::string nextSceneName;

    void switchScene(const std::string& nextSceneName);

    void setupOpenGLState(sf::Window& window);

    void restoreOpenGLState(sf::Window& window);

    void initializeOpenGL(sf::Window& window);

public:
    virtual void FixedUpdate(sf::RenderWindow& window, float fixedDeltaTime) = 0;

    virtual void Update(sf::RenderWindow& window, float deltaTime) = 0;

    virtual void Draw(sf::RenderWindow& window) = 0;

    const std::string& GetNextSceneName();

    void ResetNextSceneName();
};

#endif // SCENE_H
