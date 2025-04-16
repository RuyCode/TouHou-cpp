#include "MainMenu_Scene.h"

MainMenu_Scene::MainMenu_Scene(sf::Window& window) 
  : mainMenuFont("src/assets/fonts/La-chata-normal.ttf"),
    mainMenuText(mainMenuFont) {

    mainMenuText.setCharacterSize(24);
    mainMenuText.setFillColor(sf::Color::White);
    mainMenuText.setOutlineColor(sf::Color::Black);
    mainMenuText.setOutlineThickness(3);   
}

void MainMenu_Scene::FixedUpdate(sf::RenderWindow& window, float fixedDeltaTime) {

}

#include <iostream>

void MainMenu_Scene::Update(sf::RenderWindow& window, float deltaTime) {
    bool isArrowKeyPressed;

    auto handleArrowKey = [&](bool isPressed, int direction) {
        if (!isPressed) return;
    
        isArrowKeyPressed = true;
    
        if (!wasArrowPressedLastFrame) {
            selectedOption += direction;
        } else {
            ++scrollHoldTimer;
            if (scrollHoldTimer >= scrollInitialDelay &&
                (scrollHoldTimer - scrollInitialDelay) % scrollRepeatRate == 0) {
                selectedOption += direction;
            }
        }
    
        if (selectedOption > 7) selectedOption = 0;
        if (selectedOption < 0) selectedOption = 7;
    
        wasArrowPressedLastFrame = true;
    };
    
    handleArrowKey(sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Down), +1);
    handleArrowKey(sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Up), -1);
    
    if (!isArrowKeyPressed) {
        wasArrowPressedLastFrame = false;
        scrollHoldTimer = 0;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Z) && wasZPressedLastFrame == false) {
        switch (static_cast<MenuOption>(selectedOption)) {
            case (MenuOption::Start) : {
                switchScene("EOSD_s4");
                break;
            }
            case (MenuOption::Quit) : {
                window.close();
                break;
            }
        }

        wasZPressedLastFrame = true;
    } else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Z)) {
        wasZPressedLastFrame = false;
    }
}


void MainMenu_Scene::Draw(sf::RenderWindow& window) {
    for (int i = 0; i < 8; ++i) {
        mainMenuText.setString(menuDisplayText[i]);

        if (i == selectedOption) {
            mainMenuText.setFillColor(sf::Color::Red);
            mainMenuText.setPosition({450 - 5 * i, 200 + 30 * i - 5});
        } else {
            mainMenuText.setPosition({450 - 5 * i, 200 + 30 * i});
        }
        
        window.draw(mainMenuText);

        mainMenuText.setFillColor(sf::Color::White);
    }
}
