#include "scripts/scenes/Scene.h"

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

    std::string pauseDisplayText[3] = {
        "Resume",
        "Return to Title Screen",
        "Retry this Game"
    };
    
public:
    Pause_Scene();

    void FixedUpdate(sf::RenderWindow& window, float fixedDeltaTime) override;

    void Update(sf::RenderWindow& window, float deltaTime) override;

    void Draw(sf::RenderTarget& target) override;

    PauseResult GetPauseResult();

    void Reset();
};

#endif // PAUSE_SCENE_H