#ifndef HUD_H
#define HUD_H

#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Boss.h"

class HUD {
private:
    sf::Font font;

    sf::RectangleShape hpBarBack;
    sf::RectangleShape hpBarFront;

    sf::RectangleShape xpBarBack;
    sf::RectangleShape xpBarFront;

    sf::Text timeText;
    sf::Text finalTimeText;

    sf::Text levelText;

    sf::RectangleShape shieldBarBack;
    sf::RectangleShape shieldBarFront;

    bool showFinalTime = false;

    // --- Босс-бар ---
    sf::RectangleShape bossHpBarBack;
    sf::RectangleShape bossHpBarFront;
    sf::Text bossNameText;
    float bossBarWidth = 600.f;
    float bossBarHeight = 40.f;
    float bossBarMargin = 70.f;
    std::string currentBossName = "Hell Demon";
    bool isBossBarVisible = false;

public:
    HUD();

    void update(const Player& player, const sf::Vector2u& windowSize, float elapsedTime, const Boss* boss = nullptr);
    void draw(sf::RenderWindow& window);

    void resetFinalTime();
    std::string formatTime(float timeInSeconds);
    const sf::Font& getFont() const;

};

#endif
