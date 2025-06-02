#include "Enemy.h"
#include <iostream>
#include <cmath>
#include "Player.h"
#include "Bullet.h"

sf::Texture Enemy::ghostTexture;
sf::Texture Enemy::chechikTexture;

void Enemy::loadTextures() {
    if (ghostTexture.getSize().x == 0) {
        if (!ghostTexture.loadFromFile("assets\\ghost_final.png")) {
            std::cerr << "Error: cannot load ghost.png from assets" << std::endl;
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
}

Enemy::Enemy() : health(5) {
    loadTextures();
    enemySprite.setTexture(ghostTexture);
    enemySprite.setScale(2.15f, 2.15f);
    randomizeDirection();

    const int FRAME_WIDTH = 80;
    const int FRAME_HEIGHT = 80;
    setWalkFrameRect(0, 0, FRAME_WIDTH, FRAME_HEIGHT);
}

Enemy::Enemy(EnemyType type)
        : health(2), type(type), alive(true) {
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
    direction.x = (std::rand() % 3 - 1);
    direction.y = (std::rand() % 3 - 1);
}

void Enemy::update(float deltaTime, const sf::Vector2f& playerPosition,
                   Player& player, std::vector<Enemy>& enemies) {
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

    sf::Vector2f position = enemySprite.getPosition();

    if (directionToPlayer.x > 0.1f && !facingRight) {
        enemySprite.setScale(2.15f, 2.15f);
        enemySprite.setOrigin(0.f, 0.f);
        enemySprite.setPosition(position);
        facingRight = true;
    } else if (directionToPlayer.x < -0.1f && facingRight) {
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
            // Чтобы пули вылетали из центра врага, берем позицию хитбокса
            sf::FloatRect hb = getBounds();
            sf::Vector2f bulletStartPos(hb.left + hb.width / 2.f, hb.top + hb.height / 2.f);
            bullets.emplace_back(bulletStartPos, playerPosition);
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

    sf::FloatRect bounds = getBounds();
    sf::RectangleShape hitbox(sf::Vector2f(bounds.width, bounds.height));
    hitbox.setPosition(bounds.left, bounds.top);
    hitbox.setFillColor(sf::Color::Transparent);
    hitbox.setOutlineColor(sf::Color::Red);
    hitbox.setOutlineThickness(1.f);
    window.draw(hitbox);

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
    sf::FloatRect spriteBounds = enemySprite.getGlobalBounds();

    float marginX = spriteBounds.width * 0.2f;
    float marginY = spriteBounds.height * 0.2f;

    return sf::FloatRect(
        spriteBounds.left + marginX / 2.f,
        spriteBounds.top + marginY / 2.f,
        spriteBounds.width - marginX,
        spriteBounds.height - marginY
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
}

void Enemy::setupForMelee() {
    enemySprite.setScale(2.15f, 2.15f);
    const int FRAME_WIDTH = 90;
    const int FRAME_HEIGHT = 90;
    walkFrameCount = 4;
    setWalkFrameRect(0, 0, FRAME_WIDTH, FRAME_HEIGHT);
}
