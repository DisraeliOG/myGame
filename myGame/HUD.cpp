#include "HUD.h"
#include <iostream>

HUD::HUD() {
    if (!font.loadFromFile("fonts/DmitrievaSP.otf")) {
        std::cout << "Failed to load HUD font!" << std::endl;
    }

    // HP Bar
    hpBarBack.setSize(sf::Vector2f(200.f, 20.f));
    hpBarBack.setFillColor(sf::Color(50, 50, 50));
    hpBarBack.setPosition(20.f, 640.f); // 720p screen - bottom left

    hpBarFront.setSize(sf::Vector2f(200.f, 20.f));
    hpBarFront.setFillColor(sf::Color::Red);
    hpBarFront.setPosition(20.f, 640.f);

    // XP Bar
    xpBarBack.setSize(sf::Vector2f(200.f, 10.f));
    xpBarBack.setFillColor(sf::Color(50, 50, 50));
    xpBarBack.setPosition(20.f, 665.f);

    xpBarFront.setSize(sf::Vector2f(200.f, 10.f));
    xpBarFront.setFillColor(sf::Color::Blue);
    xpBarFront.setPosition(20.f, 665.f);
}

void HUD::update(const Player& player, const sf::Vector2u& windowSize) {
    // Расчёт позиции от нижнего края
    float x = 20.f;
    float y = static_cast<float>(windowSize.y) - 80.f;

    hpBarBack.setPosition(x, y);
    hpBarFront.setPosition(x, y);

    xpBarBack.setPosition(x, y + 25.f);
    xpBarFront.setPosition(x, y + 25.f);

    // Обновление размеров
    float healthRatio = std::max(0.f, static_cast<float>(player.getHealth()) / 100.f);
    hpBarFront.setSize(sf::Vector2f(200.f * healthRatio, 20.f));

    int currentXP = player.getExperience();
    int xpToNext = player.getExpToNextLevel();
    float xpRatio = xpToNext > 0 ? static_cast<float>(currentXP) / xpToNext : 0.f;
    xpBarFront.setSize(sf::Vector2f(200.f * xpRatio, 10.f));
}

void HUD::draw(sf::RenderWindow& window) {
    // Set default view so HUD doesn't move with camera
    sf::View originalView = window.getView();
    window.setView(window.getDefaultView());

    window.draw(hpBarBack);
    window.draw(hpBarFront);
    window.draw(xpBarBack);
    window.draw(xpBarFront);

    window.setView(originalView); // Restore game view
}