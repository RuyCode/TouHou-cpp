#include "scripts/backgrounds/EOSD_s4_Background.h"
#include "scripts/glGraphics/GLBackground.h"
#include "scripts/glGraphics/model/Model.h"
#include "scripts/glGraphics/Camera.h"
#include "scripts/glGraphics/Shader.h"
#include "scripts/glGraphics/buffers/FrameBufferObject.h"
#include "scripts/glGraphics/Texture2D.h"
#include "scripts/utils/Logger.h"
#include "scripts/utils/Env.h"

#include <iostream>
#include <glm/glm.hpp>
#include <vector>
#include <string>


const std::string EOSD_s4_Background::modelPath = "src/assets/backgroundScenes/EOSD_Stage4.glb";
const std::vector<std::string> EOSD_s4_Background::vertShaderPaths = {"src/assets/shaders/eosd_s4_0.vert", "src/assets/shaders/eosd_s4_1.vert"};
const std::vector<std::string> EOSD_s4_Background::fragShaderPaths = {"src/assets/shaders/eosd_s4_0.frag", "src/assets/shaders/eosd_s4_1.frag"};

EOSD_s4_Background::EOSD_s4_Background() : GLBackground(modelPath, vertShaderPaths, fragShaderPaths), FBO(384, 448){
    for (int i = 0; i < cameras.size(); ++i) {
        initCameraPositions.push_back(cameras[i].GetPosition());
    }

    lightPositions = {
        glm::vec3(0.f, 0.f, 0.f)
    };

    lightColors = {
        glm::vec3(1.f, 1.f, 1.f)
    };

    cameraSpeed = .11f;

    Logger::Instance().Info("Loaded gameBackground: EOSD stage 4");
}

void EOSD_s4_Background::FixedUpdate(float fixedDeltaTime) {

}

void EOSD_s4_Background::Update(float deltaTime) {
    glm::vec3 camPos = cameras[0].GetPosition();
    glm::vec3 direction(0.f, 0.f, -1.f);
    glm::vec3 newCamPos = camPos + direction * cameraSpeed * deltaTime;

    if (glm::distance(initCameraPositions[0], newCamPos) > 10.f) {
        newCamPos -= direction * 10.f;
    }

    cameras[0].SetPosition(newCamPos);
}

void EOSD_s4_Background::Draw(sf::RenderTarget& target) {
    setActiveCamera(1);

    target.clear();

    FBO.Unbind();

    FBO.BindDraw();
    glEnable(GL_SCISSOR_TEST);
    glViewport(0, 0, Env::gameViewportWidth, Env::gameViewportHeight);
    glScissor(0, 0, Env::gameViewportWidth, Env::gameViewportHeight);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    scene.Draw(shaders);
    FBO.UnbindDraw();

    target.setActive(true);
    FBO.BindRead();
    glViewport(0, 0, Env::gameViewportWidth, Env::gameViewportHeight);
    glScissor(0, 0, Env::gameViewportWidth, Env::gameViewportHeight);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBlitFramebuffer(0, 0, Env::gameViewportWidth, Env::gameViewportHeight, 0, 0, Env::gameViewportWidth, Env::gameViewportHeight, GL_COLOR_BUFFER_BIT, GL_NEAREST);
    FBO.Unbind();
    glDisable(GL_SCISSOR_TEST);

    setActiveCamera(0);

    shaders[0].Activate();

    Texture2D& frame = FBO.GetFrame();
    frame.BindToUnit(31);
    shaders[0].SetInt("background", 31);

    shaders[0].SetVec4("clearColor", glm::vec4(.502f, .247f, .129f, 1.f));
    shaders[0].SetVec3("viewPos", cameras[0].GetPosition());

    shaders[0].SetVec3("lightPositions", cameras[0].GetPosition(), lightPositions.size());
    shaders[0].SetVec3("lightColors", lightColors[0], lightColors.size());

    target.setActive(true);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_DEPTH_BUFFER_BIT);

    glViewport(0, 0, Env::gameViewportWidth, Env::gameViewportHeight);
    glScissor(0, 0, Env::gameViewportWidth, Env::gameViewportHeight);

    scene.Draw(shaders);
    target.setActive(false);
}

void EOSD_s4_Background::setActiveCamera(std::uint8_t cameraIndex) {
    const glm::mat4 viewMatrix = cameras[cameraIndex].GetViewMatrix();
    const glm::mat4 projMatrix = cameras[cameraIndex].GetPerspectiveMatrix();

    for (Shader& shader : shaders) {
        shader.Activate();
        shader.SetMat4("model", glm::mat4(1.f));
        shader.SetMat4("view", viewMatrix);
        shader.SetMat4("projection", projMatrix);
    }
}
