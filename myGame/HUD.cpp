#include "HUD.h"
#include <iostream>
#include <algorithm>

HUD::HUD() {
    if (!font.loadFromFile("fonts\\minecraft_0.ttf")) {
        std::cout << "Failed to load HUD font!" << std::endl;
    }

    hpBarBack.setSize(sf::Vector2f(200.f, 20.f));
    hpBarBack.setFillColor(sf::Color(50, 50, 50));

    hpBarFront.setSize(sf::Vector2f(200.f, 20.f));
    hpBarFront.setFillColor(sf::Color::Red);

    xpBarBack.setSize(sf::Vector2f(200.f, 10.f));
    xpBarBack.setFillColor(sf::Color(50, 50, 50));

    xpBarFront.setSize(sf::Vector2f(200.f, 10.f));
    xpBarFront.setFillColor(sf::Color::Blue);

    timeText.setFont(font);
    timeText.setCharacterSize(50);
    timeText.setFillColor(sf::Color::White);

    finalTimeText.setFont(font);
    finalTimeText.setCharacterSize(40);
    finalTimeText.setFillColor(sf::Color::White);
    finalTimeText.setStyle(sf::Text::Bold);

    levelText.setFont(font);
    levelText.setCharacterSize(30);
    levelText.setFillColor(sf::Color::White);

    shieldBarBack.setSize(sf::Vector2f(200.f, 10.f));
    shieldBarBack.setFillColor(sf::Color(30, 30, 30));

    shieldBarFront.setSize(sf::Vector2f(200.f, 10.f));
    shieldBarFront.setFillColor(sf::Color(0, 200, 255));

    bossHpBarBack.setSize(sf::Vector2f(bossBarWidth, bossBarHeight));
    bossHpBarBack.setFillColor(sf::Color(50, 50, 50, 200));
    bossHpBarBack.setOutlineThickness(2.f);
    bossHpBarBack.setOutlineColor(sf::Color::White);
    bossHpBarBack.setPosition(-1000.f, -1000.f);

    bossHpBarFront.setSize(sf::Vector2f(bossBarWidth, bossBarHeight));
    bossHpBarFront.setFillColor(sf::Color::Red);
    bossHpBarFront.setPosition(-1000.f, -1000.f);

    bossNameText.setFont(font);
    bossNameText.setCharacterSize(24);
    bossNameText.setFillColor(sf::Color::White);
    bossNameText.setString(currentBossName);
    bossNameText.setPosition(-1000.f, -1000.f);

    isBossBarVisible = false;
}

void HUD::update(const Player& player, const sf::Vector2u& windowSize, float elapsedTime, const Boss* boss) {

    std::string timeStr = "Time: " + formatTime(elapsedTime);
    timeText.setString(timeStr);

    sf::FloatRect bounds = timeText.getLocalBounds();
    timeText.setPosition(windowSize.x / 2.f - bounds.width / 2.f, 10.f);

    float x = 20.f;
    float y = static_cast<float>(windowSize.y) - 80.f;

    hpBarBack.setPosition(x, y);
    hpBarFront.setPosition(x, y);

    xpBarBack.setPosition(x, y + 25.f);
    xpBarFront.setPosition(x, y + 25.f);

    shieldBarBack.setPosition(x, y + 40.f);
    shieldBarFront.setPosition(x, y + 40.f);

    if (player.isShieldActive()) {
        float shieldRatio = std::clamp(player.getShieldRatio(), 0.f, 1.f);
        shieldBarFront.setSize(sf::Vector2f(200.f * shieldRatio, 10.f));
    } else {
        shieldBarFront.setSize(sf::Vector2f(0.f, 10.f));
    }

    float healthRatio = std::max(0.f, static_cast<float>(player.getHealth()) / player.getMaxHealth());
    hpBarFront.setSize(sf::Vector2f(200.f * healthRatio, 20.f));

    int currentXP = player.getExperience();
    int xpToNext = player.getExpToNextLevel();
    float xpRatio = xpToNext > 0 ? static_cast<float>(currentXP) / xpToNext : 0.f;
    xpBarFront.setSize(sf::Vector2f(200.f * xpRatio, 10.f));

    levelText.setString("Level: " + std::to_string(player.getLevel()));
    levelText.setPosition(20.f, y - 50.f);

    if (boss && boss->isAlive()) {
        isBossBarVisible = true;

        float bossCurrentHealth = static_cast<float>(boss->getHealth());
        float bossMaxHealth = static_cast<float>(boss->getMaxHealth());

        if (bossMaxHealth > 0) {
            float bossHealthRatio = std::clamp(bossCurrentHealth / bossMaxHealth, 0.f, 1.f);
            bossHpBarFront.setSize(sf::Vector2f(bossBarWidth * bossHealthRatio, bossBarHeight));
        } else {
            bossHpBarFront.setSize(sf::Vector2f(0.f, bossBarHeight));
        }

        bossNameText.setString(currentBossName + " (" + std::to_string(static_cast<int>(bossCurrentHealth)) + "/" + std::to_string(static_cast<int>(bossMaxHealth)) + ")");

        float posX = (windowSize.x - bossBarWidth) / 2.f;
        float posY = bossBarMargin;

        bossHpBarBack.setPosition(posX, posY);
        bossHpBarFront.setPosition(posX, posY);

        sf::FloatRect textBounds = bossNameText.getLocalBounds();
        bossNameText.setOrigin(textBounds.width / 2.f, textBounds.height / 2.f);
        bossNameText.setPosition(posX + bossBarWidth / 2.f, posY + bossBarHeight / 2.f);
    } else {
        isBossBarVisible = false;
    }
}

void HUD::draw(sf::RenderWindow& window) {
    sf::View originalView = window.getView();
    window.setView(sf::View(window.getDefaultView()));

    window.draw(hpBarBack);
    window.draw(hpBarFront);
    window.draw(xpBarBack);
    window.draw(xpBarFront);
    window.draw(shieldBarBack);
    window.draw(shieldBarFront);
    window.draw(levelText);
    window.draw(timeText);

    if (bossHpBarBack.getSize().x > 0) {
        window.draw(bossHpBarBack);
        window.draw(bossHpBarFront);
        window.draw(bossNameText);
    }

    window.setView(originalView);
}

std::string HUD::formatTime(float timeInSeconds) {
    int minutes = static_cast<int>(timeInSeconds) / 60;
    int seconds = static_cast<int>(timeInSeconds) % 60;
    return std::to_string(minutes) + ":" + (seconds < 10 ? "0" : "") + std::to_string(seconds);
}

void HUD::resetFinalTime() {
    showFinalTime = false;
}

const sf::Font& HUD::getFont() const {
    return font;
}
