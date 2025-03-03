#include "glGraphics/Model.h"
#include "glGraphics/Camera.h"
#include "glGraphics/Shader.h"

#ifndef GL_BACKGROUND_H
#define GL_BACKGROUND_H

#include <vector>

class GLBackground {
protected:
    Model scene;
    Shader shaderProgram;

    std::vector<Camera> cameras;

    float time;

    virtual void FixedUpdate(float fixedDeltaTime) = 0;

    virtual void Update(float deltaTime) = 0;

    virtual void Draw() = 0;

    GLBackground(const std::string& modelPath, const std::string& vertShaderPath, const std::string& fragShaderPath);
};

#endif // GL_BACKGROUND_H