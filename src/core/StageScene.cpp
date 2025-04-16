#include "core/StageScene.h"

StageScene::StageScene(sf::Window& window, std::shared_ptr<GLBackground> background)
  : background(background),
    collisionManager(objectManager),
    window(window),
    uiBackgroundTexture(sf::Texture("src/assets/textures/backgrounds/TouhouEchoesOfDigitalDream.png")),
    uiBackgroundSprite(sf::Sprite(uiBackgroundTexture)),
    statFont(sf::Font("src/assets/fonts/La-chata-normal.ttf")),
    statText(sf::Text(statFont)) {

    auto playerTex = std::make_shared<sf::Texture>();
    if (!playerTex->loadFromFile("src/assets/textures/Player/marisaFumo.png")) {
        throw std::runtime_error("Error: failed to load player texture");
    }

    objectManager.SetPlayer(Player(std::vector<std::shared_ptr<sf::Texture>>{playerTex}, 300.f));

    initializeOpenGL(window);

    statText.setCharacterSize(16);
    statText.setFillColor(sf::Color::White);
    statText.setOutlineColor(sf::Color::Black);
    statText.setOutlineThickness(2);    

    hiScore = 1257179610u;
    score = 1003257u;
    player = 5u;
    power = 354u;
}

void StageScene::FixedUpdate(sf::RenderWindow& window, float fixedDeltaTime) {
    objectManager.GetPlayer().FixedUpdate(fixedDeltaTime);
    background->FixedUpdate(fixedDeltaTime);
}

void StageScene::Update(sf::RenderWindow& window, float deltaTime) {
    currentFPS = 1.f / deltaTime;
    if (!isPaused) {
        objectManager.GetPlayer().Update(deltaTime);
        background->Update(deltaTime);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Escape)) {
            isPaused = true;
        }
    } else {
        pauseScene.Update(window, deltaTime);
        switch(pauseScene.GetPauseResult()) {
            case PauseResult::Resume : {
                isPaused = false;
                pauseScene.Reset();
                break;
            }

            case PauseResult::Exit : {
                isPaused = false;
                switchScene("MainMenu");
                pauseScene.Reset();
                break;
            }

            case PauseResult::None:
            default : {
                
            }
        }
    }
}

void StageScene::Draw(sf::RenderWindow& window) {
    window.draw(uiBackgroundSprite);

    statText.setCharacterSize(16);
    statText.setStyle(sf::Text::Regular);

    statText.setString("HiScore");
    sf::FloatRect bounds = statText.getLocalBounds();
    statText.setPosition({510 - bounds.size.x, 45});
    window.draw(statText);

    std::ostringstream oss;
    oss << std::setw(10) << std::setfill('0') << hiScore;
    statText.setString(oss.str());
    statText.setPosition({520, 45});
    window.draw(statText);

    statText.setString("Score");
    bounds = statText.getLocalBounds();
    statText.setPosition({510 - bounds.size.x, 70});
    window.draw(statText);

    oss.str("");
    oss << std::setw(10) << std::setfill('0') << score;
    statText.setString(oss.str());
    statText.setPosition({520, 70});
    window.draw(statText);

    statText.setString("Player");
    bounds = statText.getLocalBounds();
    statText.setPosition({510 - bounds.size.x, 105});
    window.draw(statText);

    statText.setString(L"★★★★");
    statText.setPosition({520, 105});
    window.draw(statText);

    statText.setString("Power");
    bounds = statText.getLocalBounds();
    statText.setPosition({510 - bounds.size.x, 130});
    window.draw(statText);

    statText.setString(std::to_string(power));
    statText.setPosition({520, 130});
    window.draw(statText);

    statText.setCharacterSize(8);
    statText.setStyle(sf::Text::Bold);

    statText.setString(std::to_string(currentFPS).substr(0, 4));
    statText.setPosition({600, 470});
    window.draw(statText);

    statText.setString("fps");
    statText.setPosition({620, 470});
    window.draw(statText);

    setupOpenGLState(window);
    background->Draw();
    restoreOpenGLState(window);

    objectManager.GetPlayer().Draw(window);

    if (isPaused) {
        pauseScene.Draw(window);
    }
}

