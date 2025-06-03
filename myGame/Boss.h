#pragma once
#include "Enemy.h"
#include <SFML/Audio.hpp>

class Boss : public Enemy {
public:
    Boss();
    void update(float deltaTime,
               const sf::Vector2f& playerPosition,
               Player& player,
               std::vector<std::unique_ptr<Enemy>>& enemies) override;
    void takeDamage(int damage) override;
    sf::FloatRect getBounds() const override;

    int getHealth() const { return health; }
    int getMaxHealth() const { return 500; }

private:
    void specialAttack(float deltaTime, const sf::Vector2f& playerPosition, Player& player);
    sf::Clock waveAttackCooldown;

    float specialAttackDelay = 5.f;
    sf::Clock specialAttackCooldown;
};

