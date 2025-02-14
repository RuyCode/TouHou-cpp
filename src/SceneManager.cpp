#include <SFML/Graphics.hpp>

#include "Scene.h"
#include "SceneManager.h"

SceneManager::SceneManager(const std::unordered_map<std::string, std::shared_ptr<Scene>>& sceneCache, const std::string& defaultSceneName) : sceneCache(sceneCache), currentScene(nullptr) {
    auto defaultSceneEntry = sceneCache.find(defaultSceneName);
    
    if (defaultSceneEntry != sceneCache.end()) {
        currentScene = defaultSceneEntry->second;
    } else {
        throw std::runtime_error("Error: default scene '" + defaultSceneName + "' not found");
    }
};

std::shared_ptr<Scene> SceneManager::GetCurrentScene() const {
    return currentScene;
}

void SceneManager::LoadScene(const std::string& sceneName) {
    auto sceneEntry = sceneCache.find(sceneName);

    if (sceneEntry != sceneCache.end()) {
        currentScene = sceneEntry->second;
    } else {
        throw std::runtime_error("Error: scene '" + sceneName + "' not found");
    }
};
