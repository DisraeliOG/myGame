#include "Game.h"
#include "Enemy.h"
#include "EnvironmentManager.h"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <cmath>
#include <set>
#include "UpgradeManager.h"

bool showingUpgradeMenu = false;
std::array<UpgradePtr, 3> upgradeChoices;

Game::Game()
        : window(sf::VideoMode(1280, 720), "My Game"),
          environment(window.getSize())
{
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();
    window.create(desktopMode, "Hell Yeah", sf::Style::Fullscreen);

    std::cout << "Window size: " << window.getSize().x << " x " << window.getSize().y << std::endl;

    camera.setSize(window.getSize().x, window.getSize().y);
    camera.setCenter(window.getSize().x / 2.f, window.getSize().y / 2.f);

    if (!backgroundTexture.loadFromFile("assets\\grass.png")) {
        std::cout << "Error loading background texture" << std::endl;
    }
    backgroundSprite.setTexture(backgroundTexture);
    backgroundSprite.setScale(2.f, 2.f);

    if (!selectBuffer.loadFromFile("sounds/select.wav") ||
    !shootBuffer.loadFromFile("sounds/explosion.wav") ||
    !enemyDieBuffer.loadFromFile("sounds/enemyDeath.wav") ||
    !playerHitBuffer.loadFromFile("sounds/hitHurt.wav")||
    !levelUpBuffer.loadFromFile("sounds/levelUp.wav")) {
        std::cerr << "Error loading sounds" << std::endl;
    }

    levelUpSound.setBuffer(levelUpBuffer);
    shootSound.setBuffer(shootBuffer);
    enemyDieSound.setBuffer(enemyDieBuffer);
    playerHitSound.setBuffer(playerHitBuffer);
    selectSound.setBuffer(selectBuffer);

    if (!bgm.openFromFile("sounds\\music.ogg")) {
        std::cerr << "Failed to load background music" << std::endl;
    }
    if (!deathMusic.openFromFile("sounds\\deathMusic.mp3")) {
        std::cout << "Failed to load death music\n";
    }
    bgm.setLoop(true);
    deathMusic.setLoop(true);
    bgm.play();

    waveClock.restart();
    spawnEnemies();


}

void Game::run() {
    while (window.isOpen()) {
        processEvents();
        update();
        render();
    }
}

void Game::processEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();

        if (showingUpgradeMenu && event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            sf::View originalView = window.getView();
            window.setView(window.getDefaultView());

            sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
            window.setView(originalView);

            float buttonWidth = 300.f;
            float buttonHeight = 120.f;
            float spacing = 40.f;
            float totalWidth = 3 * buttonWidth + 2 * spacing;
            float startX = (window.getSize().x - totalWidth) / 2.f;
            float y = window.getSize().y / 2.f - buttonHeight / 2.f;

            for (int i = 0; i < 3; ++i) {
                sf::FloatRect bounds(startX + i * (buttonWidth + spacing), y, buttonWidth, buttonHeight);
                if (bounds.contains(mousePos)) {
                    levelUpSound.play();
                    upgradeChoices[i]->apply(player);
                    showingUpgradeMenu = false;
                    break;
                }

            }
        }
    }
}

void Game::update() {
    if (showingUpgradeMenu || gameOver) {
        return;
    }
    lastDeltaTime = deltaClock.restart().asSeconds();

    if (player.isDead()) {
        camera.setCenter(player.getPosition());
        window.setView(camera);
        gameOver = true;
        bgm.stop();
        deathMusic.play();


        if (!finalTimeShown) {
            finalSurvivalTime = gameClock.getElapsedTime().asSeconds();
            finalTimeShown = true;
        }

        return;
    }

    player.update(enemies, lastDeltaTime);
    environment.update(player.getPosition());

    if (waveClock.getElapsedTime().asSeconds() >= timeBetweenWaves) {
        spawnEnemies();
        waveClock.restart();
    }

    for (auto& enemy : enemies) {
        if (enemy.isAlive()) {
            enemy.update(lastDeltaTime, player.getPosition(), player, enemies);
        } else if (enemy.shouldDropXp()) {
            experienceOrbs.emplace_back(enemy.getPosition());
            enemy.markXpDropped();
            enemyDieSound.play();
        }
    }

    for (auto& orb : experienceOrbs) {
        orb.update(lastDeltaTime, player.getPosition());
        if (orb.isCollected()) {
            player.addExperience(orb.getXP());
        }
    }

    if (player.hasJustLeveledUp()) {
        auto upgrades = UpgradeManager::getRandomUpgrades(3);
        for (int i = 0; i < 3; ++i) {
            upgradeChoices[i] = upgrades[i];
        }
        selectSound.play();
        showingUpgradeMenu = true;
        return;
    }


    experienceOrbs.erase(
        std::remove_if(experienceOrbs.begin(), experienceOrbs.end(),
                       [](const ExperienceOrb& orb) {
                           return orb.isCollected();
                       }),
        experienceOrbs.end()
    );

    float elapsedTime = gameClock.getElapsedTime().asSeconds();
    hud.update(player, window.getSize(), elapsedTime);

    camera.setCenter(player.getPosition());
    window.setView(camera);
}



void Game::spawnEnemies() {
    const sf::Vector2f playerPos = player.getPosition();
    const float minSpawnDistance = 1200.f;
    const float maxSpawnDistance = 2000.f;

    for (int i = 0; i < enemiesPerWave; ++i) {
        EnemyType type = (rand() % 2 == 0) ? EnemyType::Melee : EnemyType::Ranged;
        Enemy newEnemy(type);

        float angle = static_cast<float>(rand()) / RAND_MAX * 2.f * 3.1415926f;
        float distance = minSpawnDistance + static_cast<float>(rand()) / RAND_MAX * (maxSpawnDistance - minSpawnDistance);

        float x = playerPos.x + std::cos(angle) * distance;
        float y = playerPos.y + std::sin(angle) * distance;

        newEnemy.setPosition(x, y);
        enemies.push_back(newEnemy);
    }

    enemiesPerWave += 2;
    currentWave++;
}

void Game::renderDeathScreen() {
    window.setView(window.getDefaultView());
    window.clear();

    sf::Font font;
    if (!font.loadFromFile("D:\\myGame\\fonts\\minecraft_0.ttf")) {
        std::cout << "Failed to load font" << std::endl;
        return;
    }

    sf::Text deathText("YOU DIED", font, 100);
    deathText.setFillColor(sf::Color::Red);

    sf::FloatRect textRect = deathText.getLocalBounds();
    deathText.setOrigin(textRect.left + textRect.width / 2.f,
                        textRect.top + textRect.height / 2.f);
    deathText.setPosition(window.getSize().x / 2.f, window.getSize().y / 2.f - 150);

    int minutes = static_cast<int>(finalSurvivalTime) / 60;
    int seconds = static_cast<int>(finalSurvivalTime) % 60;
    std::string timeStr = "Survival time: " + std::to_string(minutes) + "m " + std::to_string(seconds) + "s";

    sf::Text timeText(timeStr, font, 40);
    timeText.setFillColor(sf::Color::White);
    sf::FloatRect timeRect = timeText.getLocalBounds();
    timeText.setOrigin(timeRect.left + timeRect.width / 2.f, timeRect.top + timeRect.height / 2.f);
    timeText.setPosition(window.getSize().x / 2.f, window.getSize().y / 2.f - 50);

    sf::Vector2f buttonSize(300, 80);
    sf::RectangleShape button(buttonSize);
    button.setFillColor(sf::Color::White);
    button.setOrigin(buttonSize.x / 2.f, buttonSize.y / 2.f);
    button.setPosition(window.getSize().x / 2.f, window.getSize().y / 2.f + 50);

    sf::Text buttonText("Play Again", font, 40);
    buttonText.setFillColor(sf::Color::Black);
    sf::FloatRect btnBounds = buttonText.getLocalBounds();
    buttonText.setOrigin(btnBounds.left + btnBounds.width / 2.f,
                         btnBounds.top + btnBounds.height / 2.f);
    buttonText.setPosition(button.getPosition());

    window.draw(deathText);
    window.draw(timeText);
    window.draw(button);
    window.draw(buttonText);
    window.display();

    sf::Event event;
    while (window.waitEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
            break;
        }
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            window.setView(window.getDefaultView());
            sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

            if (button.getGlobalBounds().contains(mousePos)) {
                restartGame();
                break;
            }
        }
    }
}

void Game::restartGame() {
    player.reset();
    enemies.clear();
    experienceOrbs.clear();
    currentWave = 1;
    enemiesPerWave = 3;
    waveClock.restart();
    gameOver = false;
    finalTimeShown = false;
    gameClock.restart();
    hud.resetFinalTime();

    camera.setCenter(player.getPosition());
    window.setView(camera);
    deathMusic.stop();
    bgm.play();
}


void Game::render() {
    window.clear();
    window.setFramerateLimit(144);

    sf::Vector2f viewCenter = camera.getCenter();
    sf::Vector2f viewSize = camera.getSize();
    sf::Vector2u textureSize = backgroundTexture.getSize();
    float scaleX = backgroundSprite.getScale().x;
    float scaleY = backgroundSprite.getScale().y;

    float texWidth = textureSize.x * scaleX;
    float texHeight = textureSize.y * scaleY;

    int left = static_cast<int>((viewCenter.x - viewSize.x / 2) / texWidth) - 1;
    int right = static_cast<int>((viewCenter.x + viewSize.x / 2) / texWidth) + 1;
    int top = static_cast<int>((viewCenter.y - viewSize.y / 2) / texHeight) - 1;
    int bottom = static_cast<int>((viewCenter.y + viewSize.y / 2) / texHeight) + 1;

    for (int x = left; x <= right; ++x) {
        for (int y = top; y <= bottom; ++y) {
            backgroundSprite.setPosition(x * texWidth, y * texHeight);
            window.draw(backgroundSprite);
        }
    }
    environment.draw(window);


    player.draw(window);
    for (const auto& bullet : player.getBullets()) {
        bullet.draw(window);
    }

    for (const auto& enemy : enemies) {
        if (enemy.isAlive()) {
            enemy.draw(window);
        }
    }
    if (gameOver) {
        renderDeathScreen();
        return;
    }

    for (const auto& orb : experienceOrbs) {
        orb.draw(window);
    }



    if (gameOver) {

    }

    hud.draw(window);

 if (showingUpgradeMenu) {
    sf::View originalView = window.getView();
    window.setView(window.getDefaultView());

    sf::Font font;
    if (!font.loadFromFile("D:\\myGame\\fonts\\minecraft_0.ttf")) {
        std::cout << "Failed to load font" << std::endl;
        return;
    }

    sf::RectangleShape overlay(sf::Vector2f(window.getSize()));
    overlay.setFillColor(sf::Color(0, 0, 0, 160));
    window.draw(overlay);

    float buttonWidth = 300.f;
    float buttonHeight = 120.f;
    float spacing = 40.f;
    float totalWidth = 3 * buttonWidth + 2 * spacing;
    float startX = (window.getSize().x - totalWidth) / 2.f;
    float y = window.getSize().y / 2.f - buttonHeight / 2.f;

    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

    for (int i = 0; i < 3; ++i) {
        sf::Vector2f pos(startX + i * (buttonWidth + spacing), y);
        sf::RectangleShape button(sf::Vector2f(buttonWidth, buttonHeight));
        button.setPosition(pos);

        sf::FloatRect bounds(pos.x, pos.y, buttonWidth, buttonHeight);
        bool hovered = bounds.contains(mousePos);
        button.setFillColor(hovered ? sf::Color(100, 100, 100, 220) : sf::Color(60, 60, 60, 200));
        button.setOutlineThickness(hovered ? 4.f : 2.f);
        button.setOutlineColor(hovered ? sf::Color::Yellow : sf::Color::White);

        sf::Text name(upgradeChoices[i]->getName(), font, 26);
        name.setFillColor(sf::Color::White);
        sf::FloatRect nameBounds = name.getLocalBounds();
        name.setOrigin(nameBounds.width / 2.f, 0);
        name.setPosition(pos.x + buttonWidth / 2.f, pos.y + 10.f);

        sf::Text desc(upgradeChoices[i]->getDescription(), font, 18);
        desc.setFillColor(sf::Color(200, 200, 200));
        sf::FloatRect descBounds = desc.getLocalBounds();
        desc.setOrigin(descBounds.width / 2.f, 0);
        desc.setPosition(pos.x + buttonWidth / 2.f, pos.y + 60.f);

        window.draw(button);
        window.draw(name);
        window.draw(desc);
    }

    window.setView(originalView);
}


    window.display();
}
