#include <SFML/Graphics.hpp>
#include <iostream>

#include "scenes/EOSD_s4_Scene.h"
#include "glGraphics/EOSD_s4_Background.h"

EOSD_s4_Scene::EOSD_s4_Scene() : background(EOSD_s4_Background()) {
    std::cout << "Loaded scene: EOSD stage 4" << std::endl;
}

void EOSD_s4_Scene::FixedUpdate(sf::RenderWindow& window, float fixedDeltaTime) {
    background.FixedUpdate(fixedDeltaTime);
}

void EOSD_s4_Scene::Update(sf::RenderWindow& window, float deltaTime) {
    background.Update(deltaTime);
}

void EOSD_s4_Scene::Draw(sf::RenderWindow& window) {
    background.Draw();
}   
