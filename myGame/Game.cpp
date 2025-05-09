#include "Game.h"
#include "Enemy.h"
#include "EnvironmentManager.h"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <cmath>
#include <set>

Game::Game()
        : window(sf::VideoMode(1280, 720), "My Game"),
          environment(window.getSize())
{
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();
    window.create(desktopMode, "Vampire Survivors Clone", sf::Style::Fullscreen);

    std::cout << "Window size: " << window.getSize().x << " x " << window.getSize().y << std::endl;

    camera.setSize(window.getSize().x, window.getSize().y);
    camera.setCenter(window.getSize().x / 2.f, window.getSize().y / 2.f);

    if (!backgroundTexture.loadFromFile("assets/grass.png")) {
        std::cout << "Error loading background texture" << std::endl;
    }
    backgroundSprite.setTexture(backgroundTexture);
    backgroundSprite.setScale(2.f, 2.f);
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
    }
}

void Game::update() {
    const float deltaTime = deltaClock.restart().asSeconds();
    if (player.isDead()) {
        camera.setCenter(player.getPosition());
        window.setView(camera);
        return;
    }
    player.update(enemies, deltaTime);
    environment.update(player.getPosition());


    if (waveClock.getElapsedTime().asSeconds() >= timeBetweenWaves) {
        spawnEnemies();
        waveClock.restart();
    }

    for (auto& enemy : enemies) {
        enemy.update(deltaTime, player.getPosition(), player, enemies);
    }
    for (auto& enemy : enemies) {
        if (enemy.isAlive()) {
            enemy.update(deltaTime, player.getPosition(), player, enemies);
        } else {
            // Drop XP if enemy just died
            static std::set<const Enemy*> alreadyDropped;
            if (alreadyDropped.find(&enemy) == alreadyDropped.end()) {
                experienceOrbs.emplace_back(enemy.getPosition());
                alreadyDropped.insert(&enemy);
            }
        }
    }

    for (auto& orb : experienceOrbs) {
        orb.update(deltaTime, player.getPosition());
        if (orb.isCollected()) {
            player.addExperience(orb.getXP());
        }
    }
    experienceOrbs.erase(
            std::remove_if(experienceOrbs.begin(), experienceOrbs.end(),
                           [](const ExperienceOrb& orb) {
                               return orb.isCollected() || orb.isExpired();
                           }),
            experienceOrbs.end());

    hud.update(player, window.getSize());

    camera.setCenter(player.getPosition());
    window.setView(camera);
}

void Game::spawnEnemies() {
    const sf::Vector2f playerPos = player.getPosition();
    const float minSpawnDistance = 1200.f;
    const float maxSpawnDistance = 2000.f;

    for (int i = 0; i < enemiesPerWave; ++i) {
        Enemy newEnemy;

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
    if (player.isDead()) {
        static sf::Font font;
        static bool loaded = font.loadFromFile("fonts/DmitrievaSP.otf");

        if (loaded) {
            sf::Text deathMessage("YOU DIED", font, 150);
            deathMessage.setFillColor(sf::Color::Red);
            deathMessage.setStyle(sf::Text::Bold);

            sf::FloatRect textRect = deathMessage.getLocalBounds();
            deathMessage.setOrigin(textRect.width / 2, textRect.height / 2);
            deathMessage.setPosition(camera.getCenter());

            window.draw(deathMessage);
        }
    }

    for (const auto& orb : experienceOrbs) {
        orb.draw(window);
    }

    hud.draw(window);

    window.display();
}
