#include "Enemy.h"
#include <iostream>
#include <cmath>
#include "Player.h"

sf::Texture Enemy::enemyTexture;

void Enemy::loadTexture() {
    if (enemyTexture.getSize().x == 0) {
        if (!enemyTexture.loadFromFile("assets/enemy.png")) {
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

void Enemy::randomizeDirection() {
    direction.x = (std::rand() % 3 - 1);
    direction.y = (std::rand() % 3 - 1);
}

void Enemy::update(float deltaTime, const sf::Vector2f& playerPosition, Player& player, std::vector<Enemy>& enemies) {
    if (!isAlive()) return;

    sf::Vector2f direction = playerPosition - enemySprite.getPosition();
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    if (length != 0) {
        direction /= length;
    }

    for (auto& otherEnemy : enemies) {
        if (&otherEnemy != this && otherEnemy.isAlive()) {
            if (this->getBounds().intersects(otherEnemy.getBounds())) {
                sf::Vector2f pushAway = enemySprite.getPosition() - otherEnemy.getPosition();
                float lengthPush = std::sqrt(pushAway.x * pushAway.x + pushAway.y * pushAway.y);

                if (lengthPush != 0) {
                    pushAway /= lengthPush;
                    enemySprite.move(pushAway * speed * deltaTime);
                }
            }
        }
    }

    enemySprite.move(direction * speed * deltaTime);

    if (enemySprite.getGlobalBounds().intersects(player.getGlobalBounds())) {
        if (attackCooldown.getElapsedTime() >= attackDelay) {
            player.takeDamage(damage);
            attackCooldown.restart();
        }
    }
}

sf::Vector2f Enemy::getPosition() const {
    return enemySprite.getPosition();
}

void Enemy::draw(sf::RenderWindow& window) const {
    window.draw(enemySprite);
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
