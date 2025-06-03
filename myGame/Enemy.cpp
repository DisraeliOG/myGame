#include "Enemy.h"
#include <iostream>
#include <cmath>
#include <algorithm>
#include "Player.h"
#include "Bullet.h"

sf::Texture Enemy::ghostTexture;
sf::Texture Enemy::chechikTexture;
sf::Texture Enemy::bossTexture;

void Enemy::loadTextures() {
    if (ghostTexture.getSize().x == 0) {
        if (!ghostTexture.loadFromFile("assets\\ghost_final.png")) {
            std::cerr << "Error: cannot load ghost_final.png from assets" << std::endl;
        } else {
            std::cout << "Ghost texture loaded OK" << std::endl;
        }
    }

    if (chechikTexture.getSize().x == 0) {
        if (!chechikTexture.loadFromFile("assets\\chechik.png")) {
            std::cerr << "Error: cannot load chechik.png from assets" << std::endl;
        } else {
            std::cout << "Chechik texture loaded OK" << std::endl;
        }
    }

    if (bossTexture.getSize().x == 0) {
        if (!bossTexture.loadFromFile("assets\\boss.png")) {
            std::cerr << "Error: cannot load boss.png from assets" << std::endl;
        } else {
            std::cout << "Boss texture loaded OK" << std::endl;
        }
    }

}




Enemy::Enemy() : health(5), alive(true) {
    loadTextures();
    enemySprite.setTexture(ghostTexture);
    enemySprite.setScale(2.15f, 2.15f);
    enemySprite.setOrigin(enemySprite.getLocalBounds().width / 2.f, enemySprite.getLocalBounds().height / 2.f);
    randomizeDirection();

    const int FRAME_WIDTH = 80;
    const int FRAME_HEIGHT = 80;
    setWalkFrameRect(0, 0, FRAME_WIDTH, FRAME_HEIGHT);
}

Enemy::Enemy(EnemyType type)
        : health(type == EnemyType::Ranged ? 2 : 5), type(type), alive(true) {
    loadTextures();

    if (type == EnemyType::Ranged) {
        enemySprite.setTexture(ghostTexture);
        attackRange = 600.0f;
        setupForRanged();
    } else {
        enemySprite.setTexture(chechikTexture);
        attackRange = 100.0f;
        setupForMelee();
    }

    randomizeDirection();
}

void Enemy::randomizeDirection() {
    direction.x = static_cast<float>(std::rand() % 3 - 1);
    direction.y = static_cast<float>(std::rand() % 3 - 1);
}

void Enemy::update(float deltaTime, const sf::Vector2f& playerPosition, Player& player, std::vector<std::unique_ptr<Enemy>>& enemies) {
    if (!isAlive()) return;

    sf::Vector2f directionToPlayer = playerPosition - enemySprite.getPosition();
    float length = std::sqrt(directionToPlayer.x * directionToPlayer.x + directionToPlayer.y * directionToPlayer.y);

    if (length != 0)
        directionToPlayer /= length;

    enemySprite.move(directionToPlayer * speed * deltaTime);

    for (auto& otherPtr : enemies) {
        if (otherPtr.get() != this && otherPtr->isAlive()) {
            if (this->getBounds().intersects(otherPtr->getBounds())) {
                sf::Vector2f pushAway = enemySprite.getPosition() - otherPtr->getPosition();
                float pushLength = std::sqrt(pushAway.x * pushAway.x + pushAway.y * pushAway.y);
                if (pushLength != 0) {
                    pushAway /= pushLength;
                    enemySprite.move(pushAway * speed * deltaTime);
                }
            }
        }
    }

    sf::Vector2f position = enemySprite.getPosition();
    float originX = enemySprite.getOrigin().x;
    float width = enemySprite.getLocalBounds().width * enemySprite.getScale().x;

    if (directionToPlayer.x > 0.1f && !facingRight) {
        enemySprite.setScale(2.15f, 2.15f);
        enemySprite.setOrigin(0.f, 0.f);
        enemySprite.setPosition(position);
        facingRight = true;
    } else if (directionToPlayer.x < -0.1f && facingRight) {
        // Повернуть влево (зеркальное отражение)
        enemySprite.setScale(-2.15f, 2.15f);
        enemySprite.setOrigin(enemySprite.getLocalBounds().width, 0.f);
        enemySprite.setPosition(position);
        facingRight = false;
    }

    if (std::abs(directionToPlayer.x) > 0.1f || std::abs(directionToPlayer.y) > 0.1f) {
        updateWalkAnimation(deltaTime);
    }

    if (type == EnemyType::Ranged) {
        if (attackCooldown.getElapsedTime().asSeconds() >= attackDelay) {
            sf::FloatRect hb = getBounds();
            sf::Vector2f bulletStartPos(hb.left + hb.width / 2.f, hb.top + hb.height / 2.f);
            float enemyBulletScale = 0.3f;
            int enemyBulletDamage = 1;

            bullets.emplace_back(bulletStartPos, playerPosition, 0.5, enemyBulletDamage, 600, bulletTexturePath);

            attackCooldown.restart();
        }

        for (auto& bullet : bullets) {
            bullet.update();
        }

        bullets.erase(
                std::remove_if(bullets.begin(), bullets.end(),
                               [&](Bullet& b) {
                                   if (b.getBounds().intersects(player.getGlobalBounds())) {
                                       player.takeDamage(b.damage);
                                       return true;
                                   }
                                   return !b.isActive;
                               }),
                bullets.end()
        );
    } else {
        if (enemySprite.getGlobalBounds().intersects(player.getGlobalBounds())) {
            if (attackCooldown.getElapsedTime().asSeconds() >= attackDelay) {
                player.takeDamage(damage);
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

    sf::FloatRect bounds = getBounds();
    sf::RectangleShape hitbox(sf::Vector2f(bounds.width, bounds.height));
    hitbox.setPosition(bounds.left, bounds.top);

    for (const auto& bullet : bullets) {
        bullet.draw(window);
    }
}

void Enemy::takeDamage(int damage) {
    health -= damage;
    if (health <= 0) {
        alive = false;
        std::cout << "Enemy defeated!" << std::endl;
    }
}

bool Enemy::isAlive() const {
    return alive;
}

sf::FloatRect Enemy::getBounds() const {
    sf::FloatRect spriteBounds = enemySprite.getGlobalBounds();

    float hitboxWidthFactor = 0.6f;
    float hitboxHeightFactor = 0.8f;
    float hitboxOffsetX = (1.f - hitboxWidthFactor) / 2.f;
    float hitboxOffsetY = (1.f - hitboxHeightFactor) / 2.f;

    return sf::FloatRect(
        spriteBounds.left + spriteBounds.width * hitboxOffsetX,
        spriteBounds.top + spriteBounds.height * hitboxOffsetY,
        spriteBounds.width * hitboxWidthFactor,
        spriteBounds.height * hitboxHeightFactor
    );
}

void Enemy::updateWalkAnimation(float deltaTime) {
    if (animationClock.getElapsedTime().asSeconds() >= frameDuration) {
        currentFrame = (currentFrame + 1) % walkFrameCount;

        int FRAME_WIDTH = walkFrameRect.width;
        int FRAME_HEIGHT = walkFrameRect.height;
        const int WALK_ROW = 0;

        setWalkFrameRect(currentFrame, WALK_ROW, FRAME_WIDTH, FRAME_HEIGHT);
        animationClock.restart();
    }
}

void Enemy::setWalkFrameRect(int col, int row, int width, int height) {
    walkFrameRect.left = col * width;
    walkFrameRect.top = row * height;
    walkFrameRect.width = width;
    walkFrameRect.height = height;
    enemySprite.setTextureRect(walkFrameRect);
}

void Enemy::setupForRanged() {
    enemySprite.setScale(2.15f, 2.15f);
    const int FRAME_WIDTH = 80;
    const int FRAME_HEIGHT = 80;
    walkFrameCount = 3;
    setWalkFrameRect(0, 0, FRAME_WIDTH, FRAME_HEIGHT);
    bulletTexturePath = "assets/enemyBullet.png";

    hitboxWidthFactor = 0.1f;
    hitboxHeightFactor = 0.1f;
    hitboxOffsetX = (1.0f - hitboxWidthFactor) / 2.0f;
    hitboxOffsetY = (1.0f - hitboxHeightFactor) / 2.0f;

}

void Enemy::setupForMelee() {
    enemySprite.setScale(2.15f, 2.15f);
    const int FRAME_WIDTH = 90;
    const int FRAME_HEIGHT = 90;
    walkFrameCount = 4;
    setWalkFrameRect(0, 0, FRAME_WIDTH, FRAME_HEIGHT);

    hitboxWidthFactor = 0.2f;
    hitboxHeightFactor = 0.9f;
    hitboxOffsetX = (1.0f - hitboxWidthFactor) / 2.0f;
    hitboxOffsetY = (1.0f - hitboxHeightFactor) / 2.0f;
}


