#ifndef PLAYER_H
#define PLAYER_H

#include <memory>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include "Bullet.h"
class Enemy;

class Player {
private:
    sf::Texture playerTexture;
    sf::Sprite playerSprite;
    sf::Vector2i frameSize = {80, 80};
    int currentFrame = 0;
    int frameCount = 5;
    float animationTimer = 0.f;
    const float frameDuration = 0.3f;

    float speed = 200.f;
    float shootDelay = 0.8f;
    sf::Clock shootClock;

    std::vector<Bullet> bullets;

    int health = 100;
    int maxHealth = 100;
    bool dead = false;

    int experience = 0;
    int level = 1;
    int expToNextLevel = 100;
    float xpGainMultiplier = 1.f;
    bool justLeveledUp = false;

    float hpRegen = 0.f;
    sf::Clock hpRegenClock;
    int vampirismHeal = 0;

    bool hasShield = false;
    int shieldHP = 0;
    int maxShieldHP = 50;
    float shieldRegenRate = 1.f;
    sf::Clock shieldRegenClock;

    // Звук
    sf::SoundBuffer shootBuffer;
    sf::Sound shootSound;

    void shootAtClosestEnemy(std::vector<std::unique_ptr<Enemy>>& enemies);

    float hitboxWidthFactor = 0.45f;
    float hitboxHeightFactor = 0.5f;
    float hitboxOffsetX = 0.25f;
    float hitboxOffsetY = 0.3f;

public:
    Player();

    void update(std::vector<std::unique_ptr<Enemy>>& enemies, float deltaTime);
    void draw(sf::RenderWindow& window);

    void handleInput();
    void reset();

    void takeDamage(int damage);

    void addExperience(int amount);

    sf::Vector2f getPosition() const;
    sf::FloatRect getGlobalBounds() const;
    const std::vector<Bullet>& getBullets() const;
    std::vector<Bullet>& getBullets();

    bool isDead() const { return dead; }
    int getHealth() const { return health; }
    int getMaxHealth() const { return maxHealth; }
    int getShieldHP() const { return shieldHP; }
    int getMaxShieldHP() const { return maxShieldHP; }
    int getExperience() const { return experience; }
    int getLevel() const { return level; }
    int getExpToNextLevel() const { return expToNextLevel; }

    bool hasJustLeveledUp();

    // Улучшения
    void increaseMaxHealth(float factor);
    void decreaseShootDelay(float factor);
    void increaseSpeed(float factor);
    void enableHpRegen(float amount);
    void enableVampirism(int heal);
    void increaseXPGainMultiplier(float factor);

    // Щит
    void enableShield();
    void updateShield(float dt);
    float getShieldRatio() const;
    sf::CircleShape getShieldVisual() const;
    bool isShieldActive() const { return hasShield && shieldHP > 0; }
};

#endif // PLAYER_H
