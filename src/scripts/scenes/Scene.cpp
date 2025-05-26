#include "scripts/scenes/Scene.h"
#include "scripts/utils/Env.h"

void Scene::setupOpenGLState() {
    glGetIntegerv(GL_ACTIVE_TEXTURE, &this->activeTextureBuffer);

    for (GLint i = 0; i < maxTextureUnits; ++i) {
        glActiveTexture(GL_TEXTURE0 + i);
        glGetIntegerv(GL_TEXTURE_BINDING_2D, &boundTexturesBuffer[i]);
    }

    glActiveTexture(activeTextureBuffer);

    glViewport(Env::gameViewportPositionX, Env::gameViewportPositionY, Env::gameViewportWidth, Env::gameViewportHeight);
    glEnable(GL_SCISSOR_TEST);
    glEnable(GL_DEPTH_TEST);

    glScissor(Env::gameViewportPositionX, Env::gameViewportPositionY, Env::gameViewportWidth, Env::gameViewportHeight);

    glm::vec4 clearColor(1.f, 1.f, 1.f, 1.f);

    glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Scene::restoreOpenGLState(sf::RenderTarget& target) {
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_SCISSOR_TEST);
    glViewport(0, 0, target.getSize().x, target.getSize().y);

    glUseProgram(0);

    for (GLint i = 0; i < maxTextureUnits; ++i) {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, boundTexturesBuffer[i]);
    }

    glActiveTexture(activeTextureBuffer);
}

void Scene::initializeOpenGL() {

    glGetIntegerv(GL_ACTIVE_TEXTURE, &this->activeTextureBuffer);
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxTextureUnits);
    boundTexturesBuffer.resize(maxTextureUnits);

    const float aspectRatio = static_cast<float>(Env::gameViewportHeight) / static_cast<float>(Env::gameViewportWidth);
}

void Scene::switchScene(const std::string& nextSceneName) {
    this->nextSceneName = nextSceneName;
}

const std::string& Scene::GetNextSceneName() {
    return nextSceneName;
}

void Scene::ResetNextSceneName() {
    nextSceneName.clear();
}