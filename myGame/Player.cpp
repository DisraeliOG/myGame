#include "Player.h"
#include "Enemy.h"
#include <algorithm>
#include <cmath>
#include <iostream>
#include "Upgrade.h"

Player::Player() {
    if (!playerTexture.loadFromFile("assets\\MCSpriteSheet.png")) {
        std::cout << "Error loading player texture" << std::endl;
    }
    playerSprite.setTexture(playerTexture);
    playerSprite.setTextureRect(sf::IntRect(0, 0, frameSize.x, frameSize.y));
    sf::FloatRect bounds = playerSprite.getLocalBounds();
    playerSprite.setOrigin(frameSize.x / 2.f, frameSize.y / 2.f);
    playerSprite.setPosition({400, 300});
    playerSprite.setScale(2.f, 2.f);

    speed = 200.f;
}

void Player::update(std::vector<Enemy>& enemies, float deltaTime) {
    sf::Vector2f movement(0.f, 0.f);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) movement.y -= 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) movement.y += 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) movement.x -= 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) movement.x += 1.f;

    if (movement != sf::Vector2f(0.f, 0.f)) {
        movement /= std::sqrt(movement.x * movement.x + movement.y * movement.y);
    }

    playerSprite.move(movement * speed * deltaTime);

    if (movement != sf::Vector2f(0.f, 0.f)) {
        animationTimer += deltaTime;
        if (animationTimer >= frameDuration) {
            animationTimer = 0.f;
            currentFrame = (currentFrame + 1) % frameCount;
            playerSprite.setTextureRect(sf::IntRect(
                    currentFrame * frameSize.x,
                    0, // строка анимации (0 - ходьба, 1 - атака и т.д.)
                    frameSize.x,
                    frameSize.y
            ));
        }

        if (movement.x < 0) {
            playerSprite.setScale(-2.f, 2.f); // влево
        } else if (movement.x > 0) {
            playerSprite.setScale(2.f, 2.f); // вправо
        }
    } else {
        currentFrame = 0;
        playerSprite.setTextureRect(sf::IntRect(0, 0, frameSize.x, frameSize.y));
    }


    Enemy* closestEnemy = nullptr;
    float closestDistanceSq = std::numeric_limits<float>::max();
    sf::Vector2f playerPos = playerSprite.getPosition();
    float shootRange = 500.f;

    for (auto& e : enemies) {
        if (!e.isAlive()) continue;

        float dx = e.getPosition().x - playerPos.x;
        float dy = e.getPosition().y - playerPos.y;
        float distSq = dx * dx + dy * dy;

        if (distSq < closestDistanceSq) {
            closestDistanceSq = distSq;
            closestEnemy = &e;
        }
    }

    if (closestEnemy && shootClock.getElapsedTime().asSeconds() >= shootDelay) {
        float dx = closestEnemy->getPosition().x - playerPos.x;
        float dy = closestEnemy->getPosition().y - playerPos.y;
        float distance = std::sqrt(dx * dx + dy * dy);

        if (distance <= shootRange) {
            bullets.emplace_back(playerPos, closestEnemy->getPosition());
            shootClock.restart();
        }
    }

    for (auto& bullet : bullets) {
        bullet.update();
        for (auto& enemy : enemies) {
            if (enemy.isAlive() && bullet.isActive && bullet.getBounds().intersects(enemy.getBounds())) {
                bullet.isActive = false;
                enemy.takeDamage();

                if (!enemy.isAlive() && vampirismHeal > 0) {
                    health = std::min(maxHealth, health + vampirismHeal);
                }

                break;
            }
        }
    }

    bullets.erase(std::remove_if(bullets.begin(), bullets.end(),
                                 [](const Bullet& b) { return !b.isActive; }),
                  bullets.end());

    if (hpRegen > 0.f && hpRegenClock.getElapsedTime().asSeconds() >= 1.f) {
        health = std::min(maxHealth, health + static_cast<int>(hpRegen));
        hpRegenClock.restart();
    }

}

void Player::draw(sf::RenderWindow &window) {
    window.draw(playerSprite);
}

void Player::takeDamage(int damage) {
    health -= damage;
    if (health <= 0) {
        health = 0;
        dead = true;
    }
    std::cout << "Player took damage! Current health: " << health << std::endl;
}

sf::Vector2f Player::getPosition() const {
    return playerSprite.getPosition();
}

sf::FloatRect Player::getGlobalBounds() const {
    return playerSprite.getGlobalBounds();
}

const std::vector<Bullet>& Player::getBullets() const {
    return bullets;
}

std::vector<Bullet>& Player::getBullets() {
    return bullets;
}

void Player::addExperience(int amount) {
    amount = static_cast<int>(amount * xpGainMultiplier);
    experience += amount;
    while (experience >= expToNextLevel) {
        experience -= expToNextLevel;
        level++;
        expToNextLevel = static_cast<int>(expToNextLevel * 1.5f);
        justLeveledUp = true;
        std::cout << "Level up! Now level " << level << std::endl;
    }
}

void Player::reset() {
    if (!playerTexture.loadFromFile("assets/MCSpriteSheet.png")) {
        std::cout << "Error loading player texture" << std::endl;
    }

    playerSprite.setTexture(playerTexture);

    playerSprite.setTextureRect(sf::IntRect(0, 0, frameSize.x, frameSize.y));

    playerSprite.setOrigin(frameSize.x / 2.f, frameSize.y / 2.f);
    playerSprite.setPosition(400, 300);
    playerSprite.setScale(2.f, 2.f);

    health = maxHealth = 100;
    experience = 0;
    level = 1;
    expToNextLevel = 100;
    speed = 200.f;
    dead = false;
    bullets.clear();
    shootClock.restart();
}

void Player::increaseMaxHealth(float factor) {
    maxHealth = static_cast<int>(maxHealth * factor);
    health = std::min(health, maxHealth);
}

void Player::decreaseShootDelay(float factor) {
    shootDelay *= factor;
}

void Player::increaseSpeed(float factor) {
    speed *= factor;
}

bool Player::hasJustLeveledUp() {
    bool temp = justLeveledUp;
    justLeveledUp = false;
    return temp;
}

void Player::enableHpRegen(float amount) {
    hpRegen += amount;
}

void Player::enableVampirism(int heal) {
    vampirismHeal += heal;
}

void Player::increaseXPGainMultiplier(float factor) {
    xpGainMultiplier *= factor;
}
