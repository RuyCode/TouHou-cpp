#include "scripts/glGraphics/GLBackground.h"

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

    void setupOpenGLState();

    void restoreOpenGLState(sf::RenderTarget& window);

    void initializeOpenGL();

public:
    virtual void FixedUpdate(sf::RenderWindow& window, float fixedDeltaTime) = 0;

    virtual void Update(sf::RenderWindow& window, float deltaTime) = 0;

    virtual void Draw(sf::RenderTarget& target) = 0;

    const std::string& GetNextSceneName();

    void ResetNextSceneName();
};

#endif // SCENE_H
