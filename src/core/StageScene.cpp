#include "core/StageScene.h"
#include "core/PlayerMarisa.h"
#include <SFML/Graphics.hpp>


StageScene::StageScene(sf::Window& window, std::shared_ptr<GLBackground> background)
  : background(background),
    objectManager(std::make_shared<ObjectManager>()),
    collisionManager(*objectManager),
    window(window),
    uiBackgroundTexture(sf::Texture("src/assets/textures/backgrounds/TouhouEchoesOfDigitalDream.png")),
    uiBackgroundSprite(sf::Sprite(uiBackgroundTexture)),
    statFont(sf::Font("src/assets/fonts/La-chata-normal.ttf")),
    statText(sf::Text(statFont)) {

    objectManager->SetPlayer(std::make_shared<PlayerMarisa>());

    initializeOpenGL();

    statText.setCharacterSize(16);
    statText.setFillColor(sf::Color::White);
    statText.setOutlineColor(sf::Color::Black);
    statText.setOutlineThickness(2);    

    hiScore = 1257179610u;
    score = 1003257u;
    player = 5u;
    power = 354u;
}

void StageScene::FixedUpdate(sf::Window& window, float fixedDeltaTime) {
    objectManager->GetPlayer()->FixedUpdate(fixedDeltaTime);
    background->FixedUpdate(fixedDeltaTime);
}

void StageScene::Update(sf::Window& window, float deltaTime) {
    currentFPS = 1.f / deltaTime;
    time += deltaTime;

    if (!isPaused) {
        objectManager->Update(deltaTime);
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

void StageScene::Draw(sf::RenderTarget& target) {
    target.draw(uiBackgroundSprite);

    statText.setCharacterSize(16);
    statText.setStyle(sf::Text::Regular);

    statText.setString("HiScore");
    sf::FloatRect bounds = statText.getLocalBounds();
    statText.setPosition({510 - bounds.size.x, 45});
    target.draw(statText);

    std::ostringstream oss;
    oss << std::setw(10) << std::setfill('0') << hiScore;
    statText.setString(oss.str());
    statText.setPosition({520, 45});
    target.draw(statText);

    statText.setString("Score");
    bounds = statText.getLocalBounds();
    statText.setPosition({510 - bounds.size.x, 70});
    target.draw(statText);

    oss.str("");
    oss << std::setw(10) << std::setfill('0') << score;
    statText.setString(oss.str());
    statText.setPosition({520, 70});
    target.draw(statText);

    statText.setString("Player");
    bounds = statText.getLocalBounds();
    statText.setPosition({510 - bounds.size.x, 105});
    target.draw(statText);

    statText.setString(L"★★★★");
    statText.setPosition({520, 105});
    target.draw(statText);

    statText.setString("Power");
    bounds = statText.getLocalBounds();
    statText.setPosition({510 - bounds.size.x, 130});
    target.draw(statText);

    statText.setString(std::to_string(power));
    statText.setPosition({520, 130});
    target.draw(statText);

    statText.setCharacterSize(8);
    statText.setStyle(sf::Text::Bold);

    statText.setString(std::to_string(currentFPS).substr(0, 4));
    statText.setPosition({600, 470});
    target.draw(statText);

    statText.setString("fps");
    statText.setPosition({620, 470});
    target.draw(statText);

    sf::ContextSettings settings;
    settings.depthBits = 24;
    sf::RenderTexture renderTexture({384, 448}, settings);
    
    setupOpenGLState();
    background->Draw(renderTexture);
    restoreOpenGLState(target);
    
    sf::Shader shader;
    std::string fragShader = isPaused ? "src/assets/shaders/pause.frag" : "src/assets/shaders/default.frag";
    if (!shader.loadFromFile("src/assets/shaders/default.vert", fragShader)) {
        throw std::runtime_error("Error: could not load shaders");
    }
    
    shader.setUniform("frame", renderTexture.getTexture());

    if (isPaused) shader.setUniform("time", time);
    
    sf::Sprite sprite(renderTexture.getTexture());
    sprite.setScale({1, -1});
    sprite.setPosition({32, 448 + 16});

    glEnable(GL_SCISSOR_TEST);
    glScissor(32, 16, 384, 448);
    objectManager->Draw(renderTexture);
    glDisable(GL_SCISSOR_TEST);

    target.draw(sprite, &shader);
    
    if (isPaused) {
        pauseScene.Draw(target);
    }
}

