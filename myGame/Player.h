#ifndef PLAYER_H
#define PLAYER_H

#include "Bullet.h"
class Enemy;
#include <SFML/Graphics.hpp>

class Player {
private:
    sf::Texture playerTexture;
    sf::Sprite playerSprite;
    std::vector<Bullet> bullets;
    sf::Clock shootClock;
    int health = 100;
    int experience = 0;
    int level = 1;
    int expToNextLevel = 100;
    float speed;
    float shootDelay = 0.8f;
    bool dead = false;

public:
    Player();
    void update(std::vector<Enemy>& enemies, float deltaTime);
    void draw(sf::RenderWindow &window);

    sf::Vector2f getPosition() const;
    const std::vector<Bullet>& getBullets() const;
    std::vector<Bullet>& getBullets();
    void handleInput();

    void takeDamage(int damage);
    int getHealth() const { return health; }
    bool isDead() const { return dead; }
    sf::FloatRect getGlobalBounds() const;

    // Опыт
    void addExperience(int amount);
    int getExperience() const { return experience; }
    int getExpToNextLevel() const { return expToNextLevel; }
    int getLevel() const { return level; }
};

#endif //PLAYER_H
