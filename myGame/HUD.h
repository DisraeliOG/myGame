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

public:
    HUD();

    void update(const Player& player, const sf::Vector2u& windowSize);
    void draw(sf::RenderWindow& window);
};

#endif // HUD_H