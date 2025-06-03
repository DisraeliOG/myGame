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
#include <SFML/Audio.hpp>
#include <memory>
#include "Boss.h"

extern bool showingUpgradeMenu;
extern std::array<UpgradePtr, 3> upgradeChoices;

class Game {
private:
    sf::RenderWindow window;
    sf::View camera;
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;
    Player player;
    EnvironmentManager environment;
    std::vector<EnvironmentObjects> environmentObjects;
    sf::Texture treeTexture;
    sf::Texture rockTexture;
    sf::Texture stickTexture;
    sf::Clock deltaClock;
    std::vector<std::unique_ptr<Enemy>> enemies;
    std::unique_ptr<Boss> currentBoss;
    int currentWave = 1;
    int enemiesPerWave = 3;
    float timeBetweenWaves = 7.f;
    sf::Clock waveClock;
    sf::Clock gameClock;
    bool finalTimeShown = false;
    float finalSurvivalTime = 0.f;
    float lastDeltaTime = 0.f;

    sf::SoundBuffer levelUpBuffer;
    sf::SoundBuffer selectBuffer;
    sf::SoundBuffer shootBuffer;
    sf::SoundBuffer enemyDieBuffer;
    sf::SoundBuffer playerHitBuffer;

    sf::Sound levelUpSound;
    sf::Sound selectSound;
    sf::Sound shootSound;
    sf::Sound enemyDieSound;
    sf::Sound playerHitSound;

    sf::Music bgm;
    sf::Music deathMusic;

    bool bossSpawned = false;
    bool bossDefeated = false;


    HUD hud;
    std::vector<ExperienceOrb> experienceOrbs;

    float deltaTime;

    void processEvents();
    void update();
    void render();

    bool gameOver = false;
    bool isBossBattle() const {
        return bossSpawned && !bossDefeated;
    }
    void spawnExperienceOrbsInCircle(const sf::Vector2f& centerPos, int count, float radius, int xpPerOrb);

public:
    Game();
    void run();
    void spawnEnemies();
    void renderDeathScreen();
    void restartGame();
    bool areAllEnemiesDefeated() const;
};
#endif
