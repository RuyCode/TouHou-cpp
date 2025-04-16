#include <SFML/Graphics.hpp>
#include <iostream>
#include <iomanip>
#include <sstream>

#include "scenes/EOSD_s4_Scene.h"
#include "backgrounds/EOSD_s4_Background.h"

EOSD_s4_Scene::EOSD_s4_Scene(sf::Window& window)
  : StageScene(window, std::make_shared<EOSD_s4_Background>()) {

    std::cout << "Loaded scene: EOSD stage 4" << std::endl;
}

void EOSD_s4_Scene::FixedUpdate(sf::RenderWindow& window, float fixedDeltaTime) {
    StageScene::Update(window, fixedDeltaTime);
}

void EOSD_s4_Scene::Update(sf::RenderWindow& window, float deltaTime) {
    StageScene::Update(window, deltaTime);
}

void EOSD_s4_Scene::Draw(sf::RenderWindow& window) {
    StageScene::Draw(window);
}
