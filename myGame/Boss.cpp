#include "Boss.h"
#include "Player.h"
#include <cmath>
#include <iostream>
#include <algorithm>
#include "Bullet.h"

const int WALK_FRAME_COUNT = 4;
const float FRAME_DURATION = 0.3f;

Boss::Boss() {
    loadTextures();
    type = EnemyType::Ranged;
    enemySprite.setTexture(bossTexture);
    enemySprite.setScale(4.0f, 4.0f);

    const int FRAME_WIDTH = 128;
    const int FRAME_HEIGHT = 128;
    enemySprite.setOrigin(FRAME_WIDTH / 2.f, FRAME_HEIGHT / 2.f);

    health = 500;
    attackRange = 800.f;
    speed = 80.f;

    setupForRanged();
    randomizeDirection();
    alive = true;

    currentFrame = 0;
    animationClock.restart();

    facingRight = true;
}

void Boss::update(float deltaTime, const sf::Vector2f& playerPosition, Player& player, std::vector<std::unique_ptr<Enemy>>& enemies) {
    if (!isAlive()) return;

    sf::Vector2f directionToPlayer = playerPosition - enemySprite.getPosition();
    float len = std::sqrt(directionToPlayer.x * directionToPlayer.x + directionToPlayer.y * directionToPlayer.y);
    if (len != 0) directionToPlayer /= len;

    enemySprite.move(directionToPlayer * speed * deltaTime);

    updateWalkAnimation(deltaTime);

    if (attackCooldown.getElapsedTime().asSeconds() >= 1.2f) {
        sf::FloatRect hb = getBounds();
        sf::Vector2f center(hb.left + hb.width / 2.f, hb.top + hb.height / 2.f);
        bullets.emplace_back(center, playerPosition, 0.5f, 3, 10000.f, bulletTexturePath);
        attackCooldown.restart();
    }

    specialAttack(deltaTime, playerPosition, player);

    if (waveAttackCooldown.getElapsedTime().asSeconds() >= 2.5f) {
        sf::Vector2f baseDir = directionToPlayer;
        sf::FloatRect hb = getBounds();
        sf::Vector2f center(hb.left + hb.width / 2.f, hb.top + hb.height / 2.f);

        for (int i = -1; i <= 1; ++i) {
            float angleOffset = i * 0.3f;
            sf::Vector2f dir(std::cos(std::atan2(baseDir.y, baseDir.x) + angleOffset),
                             std::sin(std::atan2(baseDir.y, baseDir.x) + angleOffset));
            bullets.emplace_back(center, center + dir * 5000.f, 0.5f, 2, 10000.f, bulletTexturePath);
        }
        waveAttackCooldown.restart();
    }

    for (auto& b : bullets) b.update();

    bullets.erase(std::remove_if(bullets.begin(), bullets.end(),
                                 [&](Bullet& b) {
                                     if (b.getBounds().intersects(player.getGlobalBounds())) {
                                         player.takeDamage(b.damage);
                                         return true;
                                     }
                                     return !b.isActive;
                                 }), bullets.end());

    const int FRAME_WIDTH = 128;
    const int FRAME_HEIGHT = 128;

    if (directionToPlayer.x > 0.1f && !facingRight) {
        enemySprite.setScale(4.0f, 4.0f);
        enemySprite.setOrigin(FRAME_WIDTH / 2.f, FRAME_HEIGHT / 2.f);
        facingRight = true;
    } else if (directionToPlayer.x < -0.1f && facingRight) {
        enemySprite.setScale(-4.0f, 4.0f);
        enemySprite.setOrigin(FRAME_WIDTH / 2.f, FRAME_HEIGHT / 2.f);
        facingRight = false;
    }
}

void Boss::specialAttack(float deltaTime, const sf::Vector2f& playerPosition, Player& player) {
    if (specialAttackCooldown.getElapsedTime().asSeconds() >= specialAttackDelay) {
        sf::FloatRect hb = getBounds();
        sf::Vector2f center(hb.left + hb.width / 2.f, hb.top + hb.height / 2.f);

        const int numBullets = 20;
        for (int i = 0; i < numBullets; ++i) {
            float angle = i * 2 * 3.14159f / numBullets;
            sf::Vector2f dir(std::cos(angle), std::sin(angle));
            bullets.emplace_back(center, center + dir * 5000.f, 0.5f, 3, 10000.f, bulletTexturePath);
        }
        specialAttackCooldown.restart();
    }
}

void Boss::takeDamage(int damage) {
    health -= damage;
    std::cout << "Boss takes " << damage << " damage, health left: " << health << std::endl;
    if (health <= 0) {
        std::cout << "Boss defeated!" << std::endl;
        alive = false;
    }
}

sf::FloatRect Boss::getBounds() const {
    sf::FloatRect currentBounds = enemySprite.getGlobalBounds();
    float reductionFactor = 0.25f;
    float xOffset = currentBounds.width * (1.f - reductionFactor) / 2.f;
    float yOffset = currentBounds.height * (1.f - reductionFactor) / 2.f;

    return sf::FloatRect(currentBounds.left + xOffset,
                         currentBounds.top + yOffset,
                         currentBounds.width * reductionFactor,
                         currentBounds.height * reductionFactor);
}

void Boss::updateWalkAnimation(float deltaTime) {
    if (animationClock.getElapsedTime().asSeconds() >= FRAME_DURATION) {
        currentFrame = (currentFrame + 1) % WALK_FRAME_COUNT;
        setWalkFrameRect(currentFrame, 0);
        animationClock.restart();
    }
}

void Boss::setWalkFrameRect(int col, int row) {
    const int FRAME_WIDTH = 128;
    const int FRAME_HEIGHT = 128;

    walkFrameRect.left = col * FRAME_WIDTH;
    walkFrameRect.top = row * FRAME_HEIGHT;
    walkFrameRect.width = FRAME_WIDTH;
    walkFrameRect.height = FRAME_HEIGHT;
    enemySprite.setTextureRect(walkFrameRect);
}
