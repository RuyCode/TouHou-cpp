#include "glGraphics/EOSD_s4_Background.h"

#include <iostream>

const std::string EOSD_s4_Background::modelPath = "src/assets/scenes/EOSD_s4.glb";
const std::string EOSD_s4_Background::vertShaderPath = "src/assets/shaders/eosd_s4.vert";
const std::string EOSD_s4_Background::fragShaderPath = "src/assets/shaders/eosd_s4.frag";

EOSD_s4_Background::EOSD_s4_Background() : GLBackground(modelPath, vertShaderPath, fragShaderPath) {
    for (int i = 0; i < cameras.size(); ++i) {
        initCameraPositions.push_back(cameras[i].GetPosition());
    }

    lightPositions = {
        glm::vec3(0.f, 0.f, 0.f)
    };

    lightColors = {
        glm::vec3(1.f, 1.f, 1.f)
    };


    std::cout << "Loaded background: EOSD stage 4" << std::endl;
}

void EOSD_s4_Background::FixedUpdate(float fixedDeltaTime) {

}

void EOSD_s4_Background::Update(float deltaTime) {
    glm::vec3 camPos = cameras[0].GetPosition();
    glm::vec3 direction(0.f, 0.f, -1.f);
    glm::vec3 newCamPos = camPos + direction * deltaTime;

    if (glm::distance(initCameraPositions[0], newCamPos) > 1.4f) {
        newCamPos -= direction * 1.4f;
    }

    cameras[0].SetPosition(newCamPos);
}

void EOSD_s4_Background::Draw() {
    shaderProgram.Activate();

    shaderProgram.SetFloat("time", time);


    shaderProgram.SetVec4("clearColor", glm::vec4(.502f, .247f, .129f, 1.f));
    shaderProgram.SetVec3("viewPos", cameras[0].GetPosition());

    lightPositions[0] = cameras[0].GetPosition();
    shaderProgram.SetVec3("lightPositions", lightPositions[0], lightPositions.size());

    shaderProgram.SetVec3("lightColors", lightColors[0], lightColors.size());

    glm::mat4 model = glm::mat4(1.f); 
    model = glm::rotate(model, glm::radians(time), glm::vec3(0.f, 0.f, 1.f));  

    glm::mat4 view = cameras[0].GetViewMatrix();

    glm::mat4 projection = cameras[0].GetPerspectiveMatrix();

    shaderProgram.SetMat4("model", model);
    shaderProgram.SetMat4("view", view);
    shaderProgram.SetMat4("projection", projection);

    scene.Draw(shaderProgram);
}
