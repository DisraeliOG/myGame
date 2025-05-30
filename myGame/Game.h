#ifndef GAME_H
#define GAME_H

#include "Enemy.h"
#include "EnvironmentObjects.h"
#include "Player.h"
#include <SFML/Graphics.hpp>
#include "EnvironmentManager.h"
#include "HUD.h"
#include "ExperienceOrb.h"
#include "Upgrade.h"
#include <array>

extern bool showingUpgradeMenu;
extern std::array<UpgradePtr, 3> upgradeChoices;

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
    sf::Clock gameClock;
    bool finalTimeShown = false;
    float finalSurvivalTime = 0.f;

    HUD hud;

    std::vector<ExperienceOrb> experienceOrbs;

    float deltaTime;

    void processEvents();
    void update();
    void render();

    bool gameOver = false;

public:
    Game();
    void run();
    void spawnEnemies();
    void renderDeathScreen();
    void restartGame();
    bool areAllEnemiesDefeated() const;
};

#endif //GAME_H
