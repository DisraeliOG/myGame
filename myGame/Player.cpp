#include "Player.h"
#include "Enemy.h"
#include <cmath>
#include <iostream>
#include <algorithm>

Player::Player() {
    if (!playerTexture.loadFromFile("assets/MCSpriteSheet.png"))
        std::cerr << "Error loading player texture\n";
    playerSprite.setTexture(playerTexture);
    playerSprite.setTextureRect(sf::IntRect(0, 0, frameSize.x, frameSize.y));
    playerSprite.setOrigin(frameSize.x / 2.f, frameSize.y / 2.f);
    playerSprite.setPosition(400, 300);
    playerSprite.setScale(2.f, 2.f);

    if (!shootBuffer.loadFromFile("sounds/shoot.wav"))
        std::cerr << "Error loading shoot sound\n";
    else
        shootSound.setBuffer(shootBuffer);

    if (hasShield) {
        shieldHP = maxShieldHP;
        shieldRegenClock.restart();
    }
}

void Player::update(std::vector<std::unique_ptr<Enemy>>& enemies, float deltaTime) {
    sf::Vector2f movement(0.f, 0.f);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) movement.y -= 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) movement.y += 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) movement.x -= 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) movement.x += 1.f;

    if (movement != sf::Vector2f(0.f, 0.f))
        movement /= std::sqrt(movement.x * movement.x + movement.y * movement.y);

    playerSprite.move(movement * speed * deltaTime);

    if (movement != sf::Vector2f(0.f, 0.f)) {
        animationTimer += deltaTime;
        if (animationTimer >= frameDuration) {
            animationTimer = 0.f;
            currentFrame = (currentFrame + 1) % frameCount;
            playerSprite.setTextureRect(sf::IntRect(currentFrame * frameSize.x, 0, frameSize.x, frameSize.y));
        }
        playerSprite.setScale(movement.x < 0 ? -2.f : 2.f, 2.f);
    } else {
        currentFrame = 0;
        playerSprite.setTextureRect(sf::IntRect(0, 0, frameSize.x, frameSize.y));
    }

    shootAtClosestEnemy(enemies);

    for (auto& bullet : bullets)
        bullet.update();

    for (auto& bullet : bullets) {
        if (!bullet.isActive) continue;
        for (auto& enemy : enemies) {
            if (enemy->isAlive() && bullet.getBounds().intersects(enemy->getBounds())) {
                bullet.isActive = false;
                enemy->takeDamage(bullet.damage);
                if (!enemy->isAlive() && vampirismHeal > 0)
                    health = std::min(maxHealth, health + vampirismHeal);
                break;
            }
        }
    }

    bullets.erase(std::remove_if(bullets.begin(), bullets.end(),
                                 [](const Bullet& b) { return !b.isActive; }),
                  bullets.end());

    updateShield(deltaTime);

    if (hpRegen > 0.f && hpRegenClock.getElapsedTime().asSeconds() >= 1.f) {
        health = std::min(maxHealth, health + static_cast<int>(hpRegen));
        hpRegenClock.restart();
    }
}

void Player::draw(sf::RenderWindow& window) {
    window.draw(playerSprite);
    if (hasShield && shieldHP > 0)
        window.draw(getShieldVisual());

    // Отладочный хитбокс игрока
    sf::FloatRect bounds = getGlobalBounds();
    sf::RectangleShape hitbox(sf::Vector2f(bounds.width, bounds.height));
    hitbox.setPosition(bounds.left, bounds.top);
    hitbox.setFillColor(sf::Color::Transparent);
    hitbox.setOutlineColor(sf::Color::Blue);
    hitbox.setOutlineThickness(1.f);
    window.draw(hitbox);
}

void Player::shootAtClosestEnemy(std::vector<std::unique_ptr<Enemy>>& enemies) {
    Enemy* closestEnemyPtr = nullptr;
    float minDistanceSq = std::numeric_limits<float>::max();
    sf::Vector2f playerPos = getPosition();

    for (auto& enemyPtr : enemies) {
        if (enemyPtr && enemyPtr->isAlive()) {
            sf::Vector2f enemyPos = enemyPtr->getPosition();
            float distanceSq = (enemyPos.x - playerPos.x) * (enemyPos.x - playerPos.x) +
                               (enemyPos.y - playerPos.y) * (enemyPos.y - playerPos.y);

            if (distanceSq < minDistanceSq) {
                minDistanceSq = distanceSq;
                closestEnemyPtr = enemyPtr.get();
            }
        }
    }

    if (closestEnemyPtr && std::sqrt(minDistanceSq) <= 500.f) {
        if (shootClock.getElapsedTime().asSeconds() >= shootDelay) {
            sf::FloatRect bounds = closestEnemyPtr->getBounds();
            float targetX = bounds.left + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX) / bounds.width);
            float targetY = bounds.top + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX) / bounds.height);
            sf::Vector2f target(targetX, targetY);
            sf::Vector2f dir = target - playerPos;
            float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);

            if (len != 0) {
                dir /= len;
            } else {
                dir = sf::Vector2f(1.f, 0.f);
            }

            sf::Vector2f spawnPos = playerPos + dir * 30.f;

            bullets.emplace_back(spawnPos, target, 0.5f, 10, 10000.f, "assets/bullet.gif");
            shootSound.play();
            shootClock.restart();
        }
    }
}

void Player::takeDamage(int damage) {
    if (hasShield && shieldHP > 0) {
        shieldHP -= damage;
        if (shieldHP < 0) {
            int leftover = -shieldHP;
            shieldHP = 0;
            health -= leftover;
        }
    } else {
        health -= damage;
    }

    if (health <= 0) {
        health = 0;
        dead = true;
    }
}

void Player::reset() {
    playerSprite.setPosition(400, 300);
    health = maxHealth = 100;
    experience = 0;
    level = 1;
    expToNextLevel = 100;
    speed = 200.f;
    dead = false;
    bullets.clear();
    shootClock.restart();

    if (hasShield) {
        shieldHP = maxShieldHP;
        shieldRegenClock.restart();
    }
}

sf::Vector2f Player::getPosition() const { return playerSprite.getPosition(); }

sf::FloatRect Player::getGlobalBounds() const {
    sf::FloatRect spriteBounds = playerSprite.getGlobalBounds();

    float actualHitboxWidth = spriteBounds.width * hitboxWidthFactor;
    float actualHitboxHeight = spriteBounds.height * hitboxHeightFactor;

    float offsetX = spriteBounds.width * hitboxOffsetX;
    float offsetY = spriteBounds.height * hitboxOffsetY;

    return sf::FloatRect(
        spriteBounds.left + offsetX,
        spriteBounds.top + offsetY,
        actualHitboxWidth,
        actualHitboxHeight
    );
}

const std::vector<Bullet>& Player::getBullets() const { return bullets; }
std::vector<Bullet>& Player::getBullets() { return bullets; }

void Player::addExperience(int amount) {
    amount = static_cast<int>(amount * xpGainMultiplier);
    experience += amount;
    while (experience >= expToNextLevel) {
        experience -= expToNextLevel;
        level++;
        expToNextLevel = static_cast<int>(expToNextLevel * 1.5f);
        justLeveledUp = true;
    }
}

bool Player::hasJustLeveledUp() {
    bool temp = justLeveledUp;
    justLeveledUp = false;
    return temp;
}

// --- Улучшения
void Player::increaseMaxHealth(float factor) {
    maxHealth = static_cast<int>(maxHealth * factor);
    health = std::min(health, maxHealth);
}

void Player::decreaseShootDelay(float factor) { shootDelay *= factor; }
void Player::increaseSpeed(float factor) { speed *= factor; }
void Player::enableHpRegen(float amount) { hpRegen += amount; }
void Player::enableVampirism(int heal) { vampirismHeal += heal; }
void Player::increaseXPGainMultiplier(float factor) { xpGainMultiplier *= factor; }

// --- Щит
void Player::enableShield() {
    hasShield = true;
    shieldHP = maxShieldHP;
    shieldRegenClock.restart();
}

void Player::updateShield(float dt) {
    if (hasShield && shieldHP < maxShieldHP) {
        float secondsPassed = shieldRegenClock.getElapsedTime().asSeconds();
        if (secondsPassed >= 1.f / shieldRegenRate) {
            int amountToRegen = static_cast<int>(secondsPassed * shieldRegenRate);
            shieldHP = std::min(maxShieldHP, shieldHP + amountToRegen);
            shieldRegenClock.restart();
        }
    }
}

float Player::getShieldRatio() const {
    if (!hasShield) return 0.f;
    return static_cast<float>(shieldHP) / maxShieldHP;
}

sf::CircleShape Player::getShieldVisual() const {
    sf::CircleShape shieldCircle(40.f * getShieldRatio());
    shieldCircle.setOrigin(shieldCircle.getRadius(), shieldCircle.getRadius());
    shieldCircle.setPosition(playerSprite.getPosition());
    sf::Uint8 alpha = static_cast<sf::Uint8>(150 * getShieldRatio());
    shieldCircle.setFillColor(sf::Color(0, 200, 255, alpha));
    shieldCircle.setOutlineThickness(2.f);
    shieldCircle.setOutlineColor(sf::Color(0, 200, 255, 180));
    return shieldCircle;
}


