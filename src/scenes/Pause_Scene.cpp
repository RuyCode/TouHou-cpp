#include "Pause_Scene.h"

Pause_Scene::Pause_Scene()
  : font(sf::Font("src/assets/fonts/La-chata-normal.ttf")),
    text(sf::Text(font)) {

    text.setCharacterSize(20);
    text.setOutlineColor(sf::Color::Black);
    text.setOutlineThickness(2);
}

void Pause_Scene::FixedUpdate(sf::Window& window, float fixedDeltaTime) {

}

void Pause_Scene::Update(sf::Window& window, float deltaTime) {
    bool isArrowKeyPressed;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Up)) {
        isArrowKeyPressed = true;

        if (lastPressedKey != sf::Keyboard::Scancode::Up) {
            --selectedOption;
            lastPressedKey = sf::Keyboard::Scancode::Up;
        }
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Down)) {
        isArrowKeyPressed = true;

        if (lastPressedKey != sf::Keyboard::Scancode::Down) {
            ++selectedOption;
            lastPressedKey = sf::Keyboard::Scancode::Down;
        }
    }

    if (selectedOption < 0) selectedOption = 2;
    if (selectedOption > 2) selectedOption = 0;

    if (!isArrowKeyPressed) {
        lastPressedKey = sf::Keyboard::Scancode::Unknown;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Z)) {
        pauseResult = selectedOption == 0 ? PauseResult::Resume : PauseResult::Exit;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::X)) {
        pauseResult = PauseResult::Resume;
    }
}

void Pause_Scene::Draw(sf::RenderTarget& target) {
    text.setString("Pause");
    text.setPosition({224 - text.getLocalBounds().size.x * .5f, 180});
    target.draw(text);

    for (int i = 0; i < 3; ++i) {
        text.setString(pauseDisplayText[i]);

        if (i == selectedOption) {
            text.setFillColor(sf::Color(255, 160, 160, 255));
            text.setPosition({224 - text.getLocalBounds().size.x / 2, 230 + 40 * i});
        } else {
            text.setPosition({224 - text.getLocalBounds().size.x / 2, 230 + 40 * i + 5});
        }
        
        target.draw(text);

        text.setFillColor(sf::Color::White);
    }
}

PauseResult Pause_Scene::GetPauseResult() {
    return pauseResult;
}

void Pause_Scene::Reset() {
    pauseResult = PauseResult::None;
}