#ifndef ENEMY_H
#define ENEMY_H

#include <memory>

#include "Bullet.h"
class Player;

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Clock.hpp>
#include <vector>
#include <algorithm>

enum class EnemyType {
    Melee,
    Ranged
};

class Enemy {
protected:
    static sf::Texture ghostTexture;
    static sf::Texture chechikTexture;
    sf::Sprite enemySprite;
    float speed = 100.0f;
    sf::Vector2f direction;
    int health = 10;
    int damage = 1;
    int walkFrameCount = 3;
    bool xpDropped = false;
    bool alive = true;
    float attackRange = 150.f;

    int currentFrame = 0;
    float frameDuration = 0.15f;
    sf::Clock animationClock;
    sf::IntRect walkFrameRect;
    bool facingRight = true;

    void setupForRanged();
    void setupForMelee();

    void updateWalkAnimation(float deltaTime);
    void setWalkFrameRect(int col, int row, int width, int height);

    float hitboxWidthFactor = 1.0f;
    float hitboxHeightFactor = 1.0f;
    float hitboxOffsetX = 0.0f;
    float hitboxOffsetY = 0.0f;

    std::string bulletTexturePath;

public:
    Enemy();
    Enemy(EnemyType type);
    virtual ~Enemy() = default;

    static void loadTextures();

    virtual void update(float deltaTime,
                       const sf::Vector2f& playerPosition,
                       Player& player,
                       std::vector<std::unique_ptr<Enemy>>& enemies);

    sf::Vector2f getPosition() const;
    void draw(sf::RenderWindow& window) const;
    void randomizeDirection();
    void setPosition(float x, float y) { enemySprite.setPosition(x, y); }
    virtual void takeDamage(int damage);
    bool isAlive() const;
    virtual sf::FloatRect getBounds() const;
    sf::Clock attackCooldown;
    static constexpr float attackDelay = 3.0f;
    bool shouldDropXp() const {
        return !isAlive() && !xpDropped;
    };
    void markXpDropped() {
        xpDropped = true;
    }
    EnemyType type;
    std::vector<Bullet> bullets;
};

#endif //ENEMY_H
