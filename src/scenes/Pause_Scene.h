#include "core/Scene.h"

#ifndef PAUSE_SCENE_H
#define PAUSE_SCENE_H

enum class PauseResult {
    None,
    Resume,
    Exit
};

class Pause_Scene : public Scene {
private:
    sf::Font font;
    sf::Text text;

    PauseResult pauseResult = PauseResult::None;
    std::int8_t selectedOption = 0;

    sf::Keyboard::Scan lastPressedKey = sf::Keyboard::Scan::Unknown;

    std::string pauseDisplayText[2] = {
        "No",
        "Yes"
    };
    
public:
    Pause_Scene();

    void FixedUpdate(sf::RenderWindow& window, float fixedDeltaTime) override;

    void Update(sf::RenderWindow& window, float deltaTime) override;

    void Draw(sf::RenderWindow& window) override;

    PauseResult GetPauseResult();

    void Reset();
};

#endif // PAUSE_SCENE_H