#include "scripts/glGraphics/GLBackground.h"
#include "scripts/glGraphics/model/Model.h"
#include "scripts/glGraphics/Camera.h"
#include "scripts/glGraphics/Shader.h"

#include <vector>

GLBackground::GLBackground(const std::string& modelPath, const std::vector<std::string>& vertShaderPaths, const std::vector<std::string>& fragShaderPaths) : 
    scene(Model(modelPath)), 
    cameras(scene.GetCameras()),
    time(0.f) 
{
    if (vertShaderPaths.size() != fragShaderPaths.size()) {
        throw std::runtime_error("Error: invalid shader paths for model: " + modelPath);
    }

    shaders.reserve(vertShaderPaths.size());

    for (int i = 0; i < vertShaderPaths.size(); ++i) {
        shaders.emplace_back(vertShaderPaths[i], fragShaderPaths[i]);
    }
}
