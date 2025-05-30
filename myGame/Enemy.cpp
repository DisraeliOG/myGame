#include "Enemy.h"
#include <iostream>
#include <cmath>
#include "Player.h"
#include "Bullet.h"

sf::Texture Enemy::enemyTexture;

void Enemy::loadTexture() {
    if (enemyTexture.getSize().x == 0) {
        if (!enemyTexture.loadFromFile("assets\\enemy.png")) {
            std::cerr << "Error: cannot load enemy.png from D:\\myGame\\assets" << std::endl;
        } else {
            std::cout << "Enemy texture loaded OK" << std::endl;
        }
    }
}

Enemy::Enemy() : speed(50.0f), direction(0, 0), changeDirection(2.f), health(5) {
    loadTexture();
    enemySprite.setTexture(enemyTexture);
    enemySprite.setScale(0.15f, 0.15f);
    randomizeDirection();
}

Enemy::Enemy(EnemyType type)
    : speed(50.0f), direction(0, 0), changeDirection(2.f), health(5), type(type), alive(true) {
    loadTexture();
    enemySprite.setTexture(enemyTexture);
    enemySprite.setScale(0.15f, 0.15f);
    randomizeDirection();

    if (type == EnemyType::Ranged) {
        enemySprite.setColor(sf::Color::Cyan);
        attackRange = 600.0f;
    } else {
        attackRange = 100.0f;
    }
}

void Enemy::randomizeDirection() {
    direction.x = (std::rand() % 3 - 1);
    direction.y = (std::rand() % 3 - 1);
}

void Enemy::update(float deltaTime, const sf::Vector2f& playerPosition, Player& player, std::vector<Enemy>& enemies) {
    if (!isAlive()) return;

    sf::Vector2f directionToPlayer = playerPosition - enemySprite.getPosition();
    float length = std::sqrt(directionToPlayer.x * directionToPlayer.x + directionToPlayer.y * directionToPlayer.y);

    if (length != 0)
        directionToPlayer /= length;

    enemySprite.move(directionToPlayer * speed * deltaTime);

    for (auto& otherEnemy : enemies) {
        if (&otherEnemy != this && otherEnemy.isAlive()) {
            if (this->getBounds().intersects(otherEnemy.getBounds())) {
                sf::Vector2f pushAway = enemySprite.getPosition() - otherEnemy.getPosition();
                float pushLength = std::sqrt(pushAway.x * pushAway.x + pushAway.y * pushAway.y);
                if (pushLength != 0) {
                    pushAway /= pushLength;
                    enemySprite.move(pushAway * speed * deltaTime);
                }
            }
        }
    }

    if (type == EnemyType::Ranged) {
        if (attackCooldown.getElapsedTime().asSeconds() >= attackDelay) {
            bullets.emplace_back(enemySprite.getPosition(), playerPosition);
            attackCooldown.restart();
        }

        for (auto& bullet : bullets) {
            bullet.update();
        }

        bullets.erase(
            std::remove_if(bullets.begin(), bullets.end(),
                           [&](Bullet& b) {
                               if (b.getBounds().intersects(player.getGlobalBounds())) {
                                   player.takeDamage(10);
                                   return true;
                               }
                               return !b.isActive;
                           }),
            bullets.end()
        );
    } else {
        if (enemySprite.getGlobalBounds().intersects(player.getGlobalBounds())) {
            if (attackCooldown.getElapsedTime().asSeconds() >= attackDelay) {
                player.takeDamage(10);
                attackCooldown.restart();
            }
        }
    }
}

sf::Vector2f Enemy::getPosition() const {
    return enemySprite.getPosition();
}

void Enemy::draw(sf::RenderWindow& window) const {
    window.draw(enemySprite);
    for (const auto& bullet : bullets) {
        bullet.draw(window);
    }
}

void Enemy::takeDamage() {
    health--;
    if (health <= 0) {
        std::cout << "Enemy defeated!" << std::endl;
    }
}

bool Enemy::isAlive() const {
    return health > 0;
}

sf::FloatRect Enemy::getBounds() const {
    return enemySprite.getGlobalBounds();
}
