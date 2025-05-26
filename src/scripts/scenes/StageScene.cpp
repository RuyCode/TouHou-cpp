#include "scripts/scenes/StageScene.h"
#include "scripts/entities/PlayerMarisa.h"
#include "scripts/utils/Logger.h"
#include "scripts/utils/Env.h"
#include "scripts/utils/Debug.h"

#include <SFML/Graphics.hpp>

// ---------------REMOVE LATER------------------
#include "scripts/entities/Enemy.h"
// ---------------------------------------------

StageScene::StageScene(sf::RenderWindow& window, std::shared_ptr<GLBackground> background)
  : background(background),
    objectManager(std::make_shared<ObjectManager>()),
    collisionManager(std::make_shared<CollisionManager>(objectManager)),
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

    // ------------------------ TEMP HARDCODED ENEMY SPAWN ------------------------

    std::shared_ptr<Enemy> dummyEnemy = objectManager->GetFreeEnemy();
    std::vector<glm::vec2> path = {
        {-100.f, 364.f},
        {41.f, 360.f},
        {129.f, 417.f},
        {179.f, 326.f},
        {258.f, 248.f},
        {359.f, 289.f},
        {548.f, 260.f},
    };
    
    auto defaultTex = std::make_shared<sf::Texture>();
    if (!defaultTex->loadFromFile("src/assets/textures/enemies/enemyIdle.png")) {
        throw std::runtime_error("Error: failed to load dummy enemy texture");
    }

    std::vector<std::shared_ptr<sf::Texture>> textures {
        defaultTex
    };
    
    dummyEnemy->SetPath(Spline(path, CharacteristicsMatrix::CatmullRom, 200u));
    dummyEnemy->SetTextures(textures);
    dummyEnemy->SetTravelTime(5.f);
    dummyEnemy->SetHealth(200.f);
    // ----------------------------------------------------------------------------
}

void StageScene::FixedUpdate(sf::RenderWindow& window, float fixedDeltaTime) {
    objectManager->FixedUpdate(fixedDeltaTime);
    background->FixedUpdate(fixedDeltaTime);
}

void StageScene::Update(sf::RenderWindow& window, float deltaTime) {
    currentFPS = 1.f / deltaTime;
    time += deltaTime;

    if (!isPaused) {
        objectManager->Update(deltaTime);

        auto hits = collisionManager->GetPlayerBulletToEnemyCollisions();
        for (const CollisionPair& hit : hits) {
            auto enemy = std::dynamic_pointer_cast<Enemy>(hit.object);
            enemy->SetHealth(enemy->GetHealth() - 10.f);
            hit.bullet->SetActive(false);
        }

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

    int power = objectManager->GetPlayer()->GetPower();
    int integerPart = power / 100;
    int decimalPart = power % 100;
    
    std::string formattedPower = std::to_string(integerPart) + "." + (decimalPart < 10 ? "0" : "") + std::to_string(decimalPart);
    
    statText.setString(formattedPower + " / 4.00");
    statText.setPosition({520, 130});
    target.draw(statText);
    

    if (DEBUG_MODE) {
        std::vector<std::shared_ptr<Bullet>> enemyBullets = objectManager->GetEnemyBullets();
        std::vector<std::shared_ptr<Bullet>> playerBullets = objectManager->GetPlayerBullets();

        auto isActive = [](const std::shared_ptr<GameObject>& obj) { return obj->GetActive(); };
        
        std::uint16_t enemyBulletsCount = std::count_if(enemyBullets.begin(), enemyBullets.end(), isActive);
        std::uint16_t playerBulletsCount = std::count_if(playerBullets.begin(), playerBullets.end(), isActive);

        statText.setString("Bullets: " + std::to_string(enemyBulletsCount + playerBulletsCount));
        bounds = statText.getLocalBounds();
        statText.setPosition({450, 200});
        target.draw(statText);


        std::vector<std::shared_ptr<Enemy>> enemies = objectManager->GetEnemies();

        std::uint16_t enemiesCount = std::count_if(enemies.begin(), enemies.end(), isActive);

        statText.setString("Enemies: " + std::to_string(enemiesCount));
        bounds = statText.getLocalBounds();
        statText.setPosition({450, 220});
        target.draw(statText);
    }

    statText.setCharacterSize(8);
    statText.setStyle(sf::Text::Bold);

    std::ostringstream fpsStream;
    fpsStream << std::fixed << std::setprecision(1) << currentFPS;

    statText.setString(fpsStream.str());
    statText.setPosition({595, 470});
    target.draw(statText);

    statText.setString("fps");
    statText.setPosition({620, 470});
    target.draw(statText);

    sf::ContextSettings settings;
    settings.depthBits = 24;
    sf::RenderTexture renderTexture({Env::gameViewportWidth, Env::gameViewportHeight}, settings);
    
    setupOpenGLState();
    background->Draw(renderTexture);
    restoreOpenGLState(target);
    
    sf::Shader shader;
    std::string fragShader = isPaused ? "src/assets/shaders/pause.frag" : "src/assets/shaders/default.frag";
    if (!shader.loadFromFile("src/assets/shaders/default.vert", fragShader)) {
        Logger::Instance().Error("Error: could not load shaders: 'src/assets/shaders/pause.frag' and 'src/assets/shaders/default.frag'");
        throw std::runtime_error("Error: could not load shaders: 'src/assets/shaders/pause.frag' and 'src/assets/shaders/default.frag'");
    }
    
    shader.setUniform("frame", renderTexture.getTexture());

    if (isPaused) shader.setUniform("time", time);
    
    sf::Sprite sprite(renderTexture.getTexture());
    sprite.setScale({1, -1});
    sprite.setPosition({Env::gameViewportPositionX, Env::gameViewportHeight + Env::gameViewportPositionY});

    glEnable(GL_SCISSOR_TEST);
    glScissor(Env::gameViewportPositionX, Env::gameViewportPositionY, Env::gameViewportWidth, Env::gameViewportHeight);
    objectManager->Draw(renderTexture);
    glDisable(GL_SCISSOR_TEST);

    target.draw(sprite, &shader);
    
    if (isPaused) {
        pauseScene.Draw(target);
    }
}

