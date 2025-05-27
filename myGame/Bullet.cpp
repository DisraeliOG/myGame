#include "Bullet.h"
#include <cmath>
#include <iostream>

sf::Texture Bullet::bulletTexture;

Bullet::Bullet(sf::Vector2f startPos, sf::Vector2f targetPos)
    : isActive(true) {

  if (bulletTexture.getSize().x == 0) {
    if (!bulletTexture.loadFromFile("assets\\bullet.gif")) {
      std::cerr << "Error: could not load bullet.png\n";
    }
  }

  sprite.setTexture(bulletTexture);
  sprite.setScale(0.5f, 0.5f);
  sprite.setOrigin(bulletTexture.getSize().x / 2.f, bulletTexture.getSize().y / 2.f);
  sprite.setPosition(startPos);

  sf::Vector2f dir = targetPos - startPos;
  float length = std::sqrt(dir.x * dir.x + dir.y * dir.y);
  if (length != 0)
    velocity = dir / length * speed;
  else
    velocity = {0.f, 0.f};

  float angle = std::atan2(velocity.y, velocity.x) * 180 / 3.14159265f;
  sprite.setRotation(angle);



}

void Bullet::update() {
  sprite.move(velocity);
  distanceTraveled += std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
  if (distanceTraveled >= maxRange) {
    isActive = false;
  }
}

void Bullet::draw(sf::RenderWindow& window) const {
  window.draw(sprite);
}

sf::FloatRect Bullet::getBounds() const {
  sf::FloatRect bounds = sprite.getGlobalBounds();
  float shrinkFactor = 0.1f;

  bounds.left += bounds.width * (1 - shrinkFactor) / 2;
  bounds.top += bounds.height * (1 - shrinkFactor) / 2;
  bounds.width *= shrinkFactor;
  bounds.height *= shrinkFactor;

  return bounds;
}
