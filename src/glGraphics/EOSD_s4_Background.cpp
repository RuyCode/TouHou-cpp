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

    // CAMERA MOVEMENT FOR DEBUGGING PURPOSE ////////////////////////////
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Enter)) {
        std::cout << std::endl;
        std::cout << "Position: " << cameras[1].GetPosition().x << " " << cameras[1].GetPosition().y << " " << cameras[1].GetPosition().z << std::endl;
        std::cout << "Direction: " << cameras[1].GetDirection().x << " " << cameras[1].GetDirection().y << " " << cameras[1].GetDirection().z << std::endl;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::W)) {
        cameras[1].SetPosition(cameras[1].GetPosition() + cameras[1].GetDirection() * deltaTime * 10.f);
    } 
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::S)) {
        cameras[1].SetPosition(cameras[1].GetPosition() - cameras[1].GetDirection() * deltaTime * 10.f);
    } 
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::A)) {
        glm::vec3 worldUp = glm::vec3(0.f, 1.f, 0.f);
        glm::vec3 cameraRight = glm::cross(worldUp, cameras[1].GetDirection());
        cameras[1].SetPosition(cameras[1].GetPosition() + cameraRight * deltaTime * 10.f);
    } 
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::D)) {
        glm::vec3 worldUp = glm::vec3(0.f, 1.f, 0.f);
        glm::vec3 cameraRight = glm::cross(worldUp, cameras[1].GetDirection());
        cameras[1].SetPosition(cameras[1].GetPosition() - cameraRight * deltaTime * 10.f);
    } 
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Space)) {
        glm::vec3 worldUp = glm::vec3(0.f, 1.f, 0.f);
        glm::vec3 cameraRight = glm::cross(worldUp, cameras[1].GetDirection());
        glm::vec3 cameraUp = glm::cross(cameraRight, cameras[1].GetDirection());
        cameras[1].SetPosition(cameras[1].GetPosition() - cameraUp * deltaTime * 10.f);
    } 
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::LControl)) {
        glm::vec3 worldUp = glm::vec3(0.f, 1.f, 0.f);
        glm::vec3 cameraRight = glm::cross(worldUp, cameras[1].GetDirection());
        glm::vec3 cameraUp = glm::cross(cameraRight, cameras[1].GetDirection());
        cameras[1].SetPosition(cameras[1].GetPosition() + cameraUp * deltaTime * 10.f);
    } 

    auto rotateCamera = [](float angle, const glm::vec3& axis, Camera& cam) {
        glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(angle), axis);
        cam.SetDirection(glm::normalize(glm::vec3(rotationMatrix * glm::vec4(cam.GetDirection(), 1.0f))));
    };

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Left)) {
        glm::vec3 worldUp = glm::vec3(0.f, 1.f, 0.f);
        rotateCamera(40.f * deltaTime, worldUp, cameras[1]);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Right)) {
        glm::vec3 worldUp = glm::vec3(0.f, 1.f, 0.f);
        rotateCamera(-40.f * deltaTime, worldUp, cameras[1]);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Up)) {
        glm::vec3 worldUp = glm::vec3(0.f, 1.f, 0.f);
        glm::vec3 cameraRight = glm::normalize(glm::cross(worldUp, cameras[1].GetDirection()));
        rotateCamera(-40.f * deltaTime, cameraRight, cameras[1]); 
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Down)) {
        glm::vec3 worldUp = glm::vec3(0.f, 1.f, 0.f);
        glm::vec3 cameraRight = glm::normalize(glm::cross(worldUp, cameras[1].GetDirection()));
        rotateCamera(40.f * deltaTime, cameraRight, cameras[1]);
    }

    /////////////////////////////////////////////////////////////
}

void EOSD_s4_Background::Draw() {
    setActiveCamera(1);

    FBO.Unbind();

    scene.Draw(shaders);
    FBO.BindDraw();
    glClear(GL_COLOR_BUFFER_BIT);
    glBlitFramebuffer(0, 0, 384, 448, 0, 0, 384, 448, GL_COLOR_BUFFER_BIT, GL_NEAREST);
    FBO.UnbindDraw();

    FBO.BindRead();
    glBlitFramebuffer(0, 0, 384, 448, 0, 0, 384, 448, GL_COLOR_BUFFER_BIT, GL_NEAREST);
    FBO.UnbindRead();

    setActiveCamera(0);

    shaders[0].Activate();

    Texture2D& frame = FBO.GetFrame();
    
    frame.BindToUnit(15);
    shaders[0].SetInt("background", 15);

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
