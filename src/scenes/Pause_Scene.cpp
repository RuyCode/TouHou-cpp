#include "Pause_Scene.h"

Pause_Scene::Pause_Scene()
  : font(sf::Font("src/assets/fonts/La-chata-normal.ttf")),
    text(sf::Text(font)) {

}

void Pause_Scene::FixedUpdate(sf::RenderWindow& window, float fixedDeltaTime) {

}

void Pause_Scene::Update(sf::RenderWindow& window, float deltaTime) {
    bool isArrowKeyPressed;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Left)) {
        isArrowKeyPressed = true;

        if (lastPressedKey != sf::Keyboard::Scancode::Left) {
            --selectedOption;
            lastPressedKey = sf::Keyboard::Scancode::Left;
        }
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Right)) {
        isArrowKeyPressed = true;

        if (lastPressedKey != sf::Keyboard::Scancode::Right) {
            --selectedOption;
            lastPressedKey = sf::Keyboard::Scancode::Right;
        }
    }

    if (selectedOption < 0) selectedOption = 1;
    if (selectedOption > 1) selectedOption = 0;

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

void Pause_Scene::Draw(sf::RenderWindow& window) {
    sf::RectangleShape quad({384, 448});
    quad.setPosition({32, 16});
    quad.setFillColor(sf::Color(0, 0, 0, 128));

    window.draw(quad);

    text.setString("Exit to menu?");
    text.setPosition({224 - text.getLocalBounds().size.x * .5f, 180});
    window.draw(text);

    for (int i = 0; i < 2; ++i) {
        text.setString(pauseDisplayText[i]);

        if (i == selectedOption) {
            text.setFillColor(sf::Color::Red);
            text.setPosition({224 - text.getLocalBounds().size.x * .5f - 60 * (1 - 2 * i), 230});
        } else {
            text.setPosition({224 - text.getLocalBounds().size.x * .5f - 60 * (1 - 2 * i), 230 + 5});
        }
        
        window.draw(text);

        text.setFillColor(sf::Color::White);
    }
}

PauseResult Pause_Scene::GetPauseResult() {
    return pauseResult;
}

void Pause_Scene::Reset() {
    pauseResult = PauseResult::None;
}