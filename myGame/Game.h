#ifndef GAME_H
#define GAME_H

#include "Enemy.h"
#include "EnvironmentObjects.h"
#include "Player.h"
#include <SFML/Graphics.hpp>
#include "EnvironmentManager.h"
#include "HUD.h"
#include "ExperienceOrb.h"

class Game {
private:
    sf::RenderWindow window;
    sf::View camera;
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;
    Player player;
    Enemy enemy;
    EnvironmentManager environment;
    std::vector<EnvironmentObjects> environmentObjects;
    sf::Texture treeTexture;
    sf::Texture rockTexture;
    sf::Texture stickTexture;
    sf::Clock deltaClock;
    std::vector<Enemy> enemies;
    int currentWave = 1;
    int enemiesPerWave = 3;
    float timeBetweenWaves = 5.f;
    sf::Clock waveClock;

    HUD hud;

    std::vector<ExperienceOrb> experienceOrbs;

    float deltaTime;

    void processEvents();
    void update();
    void render();

public:
    Game();
    void run();
    void spawnEnemies();
    bool areAllEnemiesDefeated() const;
};

#endif //GAME_H
