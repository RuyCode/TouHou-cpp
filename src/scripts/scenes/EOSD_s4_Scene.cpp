#include <SFML/Graphics.hpp>
#include <iostream>
#include <iomanip>
#include <sstream>

#include "scripts/scenes/EOSD_s4_Scene.h"
#include "scripts/backgrounds/EOSD_s4_Background.h"
#include "scripts/utils/Logger.h"

EOSD_s4_Scene::EOSD_s4_Scene(sf::RenderWindow& window)
  : StageScene(window, std::make_shared<EOSD_s4_Background>()) {

    Logger::Instance().Info("Loaded scene: EOSD stage 4");
}

void EOSD_s4_Scene::FixedUpdate(sf::RenderWindow& window, float fixedDeltaTime) {
    StageScene::FixedUpdate(window, fixedDeltaTime);
}

void EOSD_s4_Scene::Update(sf::RenderWindow& window, float deltaTime) {
    StageScene::Update(window, deltaTime);
}

void EOSD_s4_Scene::Draw(sf::RenderTarget& target) {
    StageScene::Draw(target);
}