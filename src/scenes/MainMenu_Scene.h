#include "core/Scene.h"

#ifndef MAIN_MENU_SCENE
#define MAIN_MENU_SCENE

class MainMenu_Scene : public Scene {
private:
    enum class MenuOption {
        Start = 0,
        CusomStart = 1,
        PracticeStart = 2,
        Replay = 3,
        Score = 4,
        MusicRoom = 5,
        Option = 6,
        Quit = 7
    };

    std::string menuDisplayText[8] = {
        "Start",
        "Custom Start",
        "Practice Start",
        "Replay",
        "Score",
        "Music Room",
        "Option",
        "Quit"
    };

    sf::Font mainMenuFont;
    sf::Text mainMenuText;

    std::int8_t selectedOption = 0;

    const std::uint8_t scrollInitialDelay = 30;
    const std::uint8_t scrollRepeatRate = 15;
    std::uint8_t scrollHoldTimer = 0;
    bool wasArrowPressedLastFrame = false;

    bool wasZPressedLastFrame = false;

public:
    MainMenu_Scene(sf::Window& window);

    void FixedUpdate(sf::Window& window, float fixedDeltaTime) override;

    void Update(sf::Window& window, float deltaTime) override;

    void Draw(sf::RenderTarget& target) override;
};

#endif // MAIN_MENU_SCENE