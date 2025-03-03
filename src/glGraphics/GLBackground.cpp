#include "GLBackground.h"

GLBackground::GLBackground(const std::string& modelPath, const std::string& vertShaderPath, const std::string& fragShaderPath) : 
    scene(Model(modelPath)), 
    shaderProgram(Shader(vertShaderPath, fragShaderPath)),
    cameras(scene.GetCameras()),
    time(0.f) {
}