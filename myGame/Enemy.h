#ifndef ENEMY_H
#define ENEMY_H
#include "Bullet.h"
class Player;

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
enum class EnemyType {
  Melee,
  Ranged
};

class Enemy {
private:
    static sf::Texture enemyTexture;
    sf::Sprite enemySprite;
    float speed = 1;
    sf::Vector2f direction;
    sf::Clock movementClock;
    float changeDirection;
    int health;
    int damage = 1;
    bool xpDropped = false;
    bool alive = true;
    float attackRange;

public:
    Enemy();
    Enemy(EnemyType type);
    static void loadTexture();
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
    static constexpr float attackDelay = 2.0f;
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
