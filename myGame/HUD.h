#ifndef HUD_H
#define HUD_H

#include <SFML/Graphics.hpp>
#include "Player.h"

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

    bool showFinalTime = false;

public:
    HUD();

    void update(const Player& player, const sf::Vector2u& windowSize, float elapsedTime);
    void draw(sf::RenderWindow& window);

    void resetFinalTime();
    std::string formatTime(float timeInSeconds);
};

#endif
