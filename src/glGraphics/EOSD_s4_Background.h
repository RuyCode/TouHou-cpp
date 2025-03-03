#include "glGraphics/GLBackground.h"
#include "glGraphics/Model.h"
#include "glGraphics/Camera.h"
#include "glGraphics/Shader.h"

#ifndef ESOD_S4_BACKGROUND_H
#define ESOD_S4_BACKGROUND_H

#include <glm/glm.hpp>
#include <vector>
#include <string>

class EOSD_s4_Background : GLBackground {
private:
    static const std::string modelPath;
    static const std::string vertShaderPath;
    static const std::string fragShaderPath;

    std::vector<glm::vec3> initCameraPositions;

    std::vector<glm::vec3> lightPositions;
    std::vector<glm::vec3> lightColors;

public:
    EOSD_s4_Background();

    void FixedUpdate(float fixedDeltaTime) override;

    void Update(float deltaTime) override;

    void Draw() override;
};

#endif // ESOD_S4_BACKGROUND_H