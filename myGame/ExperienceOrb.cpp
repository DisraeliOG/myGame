#include "ExperienceOrb.h"
#include <cmath>

ExperienceOrb::ExperienceOrb(const sf::Vector2f& pos, int xp)
    : position(pos), xpAmount(xp)
{
    orb.setRadius(radius);
    orb.setOrigin(radius, radius);
    orb.setFillColor(sf::Color::Green);
    orb.setPosition(position);
}

void ExperienceOrb::update(float deltaTime, const sf::Vector2f& playerPos) {
    const float pickupRange = 30.f;
    sf::Vector2f dir = playerPos - position;
    float dist = std::hypot(dir.x, dir.y);

    if (dist < pickupRange) {
        collected = true;
    } else if (dist < 150.f) {
        dir /= dist;
        position += dir * 100.f * deltaTime;
        orb.setPosition(position);
    }
}

void ExperienceOrb::draw(sf::RenderWindow& window) const {
    if (!collected)
        window.draw(orb);
}

bool ExperienceOrb::isCollected() const { return collected; }
sf::FloatRect ExperienceOrb::getBounds() const { return orb.getGlobalBounds(); }
int ExperienceOrb::getXP() const      { return xpAmount; }
