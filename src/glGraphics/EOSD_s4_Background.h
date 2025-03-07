#include "glGraphics/GLBackground.h"
#include "glGraphics/Model.h"
#include "glGraphics/Camera.h"
#include "glGraphics/Shader.h"
#include "glGraphics/FrameBufferObject.h"
#include "glGraphics/Texture2D.h"

#ifndef ESOD_S4_BACKGROUND_H
#define ESOD_S4_BACKGROUND_H

#include <glm/glm.hpp>
#include <vector>
#include <string>

class EOSD_s4_Background : GLBackground {
private:
    static const std::string modelPath;
    static const std::vector<std::string> vertShaderPaths;
    static const std::vector<std::string> fragShaderPaths;

    FrameBufferObject FBO;

    std::vector<glm::vec3> initCameraPositions;

    std::vector<glm::vec3> lightPositions;
    std::vector<glm::vec3> lightColors;

    float cameraSpeed;

    void setActiveCamera(std::uint8_t cameraIndex);

public:
    EOSD_s4_Background();

    void FixedUpdate(float fixedDeltaTime) override;

    void Update(float deltaTime) override;

    void Draw() override;
};

#endif // ESOD_S4_BACKGROUND_H