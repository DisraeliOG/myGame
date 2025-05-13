#include "HUD.h"
#include <iostream>

HUD::HUD() {
    if (!font.loadFromFile("fonts/DmitrievaSP.otf")) {
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
}

void HUD::update(const Player& player, const sf::Vector2u& windowSize, float elapsedTime) {
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

    float healthRatio = std::max(0.f, static_cast<float>(player.getHealth()) / 100.f);
    hpBarFront.setSize(sf::Vector2f(200.f * healthRatio, 20.f));

    int currentXP = player.getExperience();
    int xpToNext = player.getExpToNextLevel();
    float xpRatio = xpToNext > 0 ? static_cast<float>(currentXP) / xpToNext : 0.f;
    xpBarFront.setSize(sf::Vector2f(200.f * xpRatio, 10.f));

    levelText.setString("Level: " + std::to_string(player.getLevel()));
    levelText.setPosition(20.f, y - 50.f);
}

void HUD::draw(sf::RenderWindow& window) {
    sf::View originalView = window.getView();
    window.setView(window.getDefaultView());

    window.draw(hpBarBack);
    window.draw(hpBarFront);
    window.draw(xpBarBack);
    window.draw(xpBarFront);
    window.draw(levelText);
    window.draw(timeText);
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
