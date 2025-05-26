#include "scripts/glGraphics/model/Model.h"
#include "scripts/glGraphics/Camera.h"
#include "scripts/glGraphics/Shader.h"

#ifndef GL_BACKGROUND_H
#define GL_BACKGROUND_H

#include <vector>

class GLBackground {
protected:
    Model scene;
    std::vector<Shader> shaders;

    std::vector<Camera> cameras;

    float time;
    
public:
    virtual void FixedUpdate(float fixedDeltaTime) = 0;

    virtual void Update(float deltaTime) = 0;

    virtual void Draw(sf::RenderTarget& target) = 0;

    GLBackground(const std::string& modelPath, const std::vector<std::string>& vertShaderPath, const std::vector<std::string>& fragShaderPath);
};

#endif // GL_BACKGROUND_H
