#ifndef ENEMY_H
#define ENEMY_H

#include "Bullet.h"
class Player;

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Clock.hpp>

enum class EnemyType {
    Melee,
    Ranged
};

class Enemy {
private:
    static sf::Texture ghostTexture;
    static sf::Texture chechikTexture;
    sf::Sprite enemySprite;
    float speed = 100.0f;
    sf::Vector2f direction;
    int health;
    int damage = 1;
    int walkFrameCount = 3;
    bool xpDropped = false;
    bool alive = true;
    float attackRange;

    int currentFrame = 0;
    float frameDuration = 0.15f;
    sf::Clock animationClock;
    sf::IntRect walkFrameRect;
    bool facingRight = true;

    void setupForRanged();
    void setupForMelee();

    void updateWalkAnimation(float deltaTime);
    void setWalkFrameRect(int col, int row, int width, int height);

public:
    Enemy();
    Enemy(EnemyType type);
    static void loadTextures();
    void update(float deltaTime, const sf::Vector2f &playerPosition,
                Player &player, std::vector<Enemy> &enemies);
    sf::Vector2f getPosition() const;
    void draw(sf::RenderWindow& window) const;
    void randomizeDirection();
    void setPosition(float x, float y) { enemySprite.setPosition(x, y); }
    void takeDamage();
    bool isAlive() const;
    sf::FloatRect getBounds() const;
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
