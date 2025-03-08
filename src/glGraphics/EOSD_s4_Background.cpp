#include "glGraphics/EOSD_s4_Background.h"
#include "glGraphics/FrameBufferObject.h"

#include <iostream>

const std::string EOSD_s4_Background::modelPath = "src/assets/scenes/EOSD_Stage4.glb";
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

    std::cout << "Loaded background: EOSD stage 4" << std::endl;
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

void EOSD_s4_Background::Draw() {
    setActiveCamera(1);

    FBO.Unbind();

    FBO.BindDraw();
    glEnable(GL_SCISSOR_TEST);
    glViewport(0, 0, 384, 448);
    glScissor(0, 0, 384, 448);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    scene.Draw(shaders);

    glDisable(GL_SCISSOR_TEST);
    FBO.UnbindDraw();

    FBO.BindRead();
    glEnable(GL_SCISSOR_TEST);
    glViewport(32, 16, 384, 448);
    glScissor(32, 16, 384, 448);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glBlitFramebuffer(0, 0, 384, 448, 32, 16, 384 + 32, 448 + 16, GL_COLOR_BUFFER_BIT, GL_NEAREST);
    glDisable(GL_SCISSOR_TEST);
    FBO.UnbindRead();

    setActiveCamera(0);

    shaders[0].Activate();

    Texture2D& frame = FBO.GetFrame();

    frame.BindToUnit(31);
    shaders[0].SetInt("background", 31);

    shaders[0].SetVec4("clearColor", glm::vec4(.502f, .247f, .129f, 1.f));
    shaders[0].SetVec3("viewPos", cameras[0].GetPosition());

    lightPositions[0] = cameras[0].GetPosition();
    shaders[0].SetVec3("lightPositions", lightPositions[0], lightPositions.size());

    shaders[0].SetVec3("lightColors", lightColors[0], lightColors.size());

    glClear(GL_DEPTH_BUFFER_BIT);
    scene.Draw(shaders);
}

void EOSD_s4_Background::setActiveCamera(std::uint8_t cameraIndex) {
    for (unsigned int i = 0; i < shaders.size(); ++i) {
        shaders[i].Activate();

        shaders[i].SetMat4("model", glm::mat4(1.f));
        shaders[i].SetMat4("view", cameras[cameraIndex].GetViewMatrix());
        shaders[i].SetMat4("projection", cameras[cameraIndex].GetPerspectiveMatrix());
    }
}