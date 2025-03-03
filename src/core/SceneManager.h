#include "scenes/Scene.h"

#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include <SFML/Graphics.hpp>
#include <unordered_map>

class SceneManager {
private:
    std::shared_ptr<Scene> currentScene;
    std::unordered_map<std::string, std::shared_ptr<Scene>> sceneCache;
public:
    SceneManager();

    SceneManager(const std::unordered_map<std::string, std::shared_ptr<Scene>>& sceneCache, const std::string& defaultSceneName);

    std::shared_ptr<Scene> GetCurrentScene() const;

    void LoadScene(const std::string& sceneName);
};

#endif // SCENE_MANAGER_H