#include "core/Scene.h"

void Scene::setupOpenGLState(sf::Window& window) {
    glGetIntegerv(GL_ACTIVE_TEXTURE, &this->activeTextureBuffer);

    for (GLint i = 0; i < maxTextureUnits; ++i) {
        glActiveTexture(GL_TEXTURE0 + i);
        glGetIntegerv(GL_TEXTURE_BINDING_2D, &boundTexturesBuffer[i]);
    }

    glActiveTexture(activeTextureBuffer);

    glViewport(32, 16, 384, 448);
    glEnable(GL_SCISSOR_TEST);
    glEnable(GL_DEPTH_TEST);

    glScissor(32, 16, 384, 448);

    glm::vec4 clearColor(1.f, 1.f, 1.f, 1.f);

    glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Scene::restoreOpenGLState(sf::Window& window) {
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_SCISSOR_TEST);
    glViewport(0, 0, window.getSize().x, window.getSize().y);

    glUseProgram(0);

    for (GLint i = 0; i < maxTextureUnits; ++i) {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, boundTexturesBuffer[i]);
    }

    glActiveTexture(activeTextureBuffer);
}

void Scene::initializeOpenGL(sf::Window& window) {

    glGetIntegerv(GL_ACTIVE_TEXTURE, &this->activeTextureBuffer);
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxTextureUnits);
    boundTexturesBuffer.resize(maxTextureUnits);

    const float aspectRatio = 448.f / 384.f;
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