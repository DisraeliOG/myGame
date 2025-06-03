#include "Bullet.h"
#include <cmath>
#include <iostream>


std::map<std::string, sf::Texture> Bullet::textureCache;



const sf::Texture& Bullet::getTexture(const std::string& path) {

    auto it = textureCache.find(path);
    if (it != textureCache.end()) {
        return it->second;
    } else {
        sf::Texture newTexture;
        if (!newTexture.loadFromFile(path)) {
            std::cerr << "Error: could not load bullet texture from " << path << "\n";
        }
        textureCache[path] = newTexture;
        return textureCache[path];
    }
}


Bullet::Bullet(sf::Vector2f startPos, sf::Vector2f targetPos, float scale, int damage, float maxRange, const std::string& texturePath)
    : isActive(true), startPosition(startPos), damage(damage), initialMaxDistance(maxRange) {

    const sf::Texture& texture = getTexture(texturePath);

    sprite.setTexture(texture);
    sprite.setScale(scale, scale);
    sprite.setOrigin(texture.getSize().x / 2.f, texture.getSize().y / 2.f);
    sprite.setPosition(startPos);

    sf::Vector2f dir = targetPos - startPos;
    float length = std::sqrt(dir.x * dir.x + dir.y * dir.y);

    if (length != 0) {
      velocity = dir / length * speed;
    } else {
      velocity = {speed, 0.f};
    }

    float angle = std::atan2(velocity.y, velocity.x) * 180.f / 3.14159265f;
    sprite.setRotation(angle);
}


void Bullet::update() {
  sprite.move(velocity);

  sf::Vector2f currentPos = sprite.getPosition();
  sf::Vector2f diff = currentPos - startPosition;
  float traveledDistance = std::sqrt(diff.x * diff.x + diff.y * diff.y);

  if (traveledDistance >= initialMaxDistance) {
    isActive = false;
  }
}

void Bullet::draw(sf::RenderWindow& window) const {
  if (isActive) {
      window.draw(sprite);
  }
}

sf::FloatRect Bullet::getBounds() const {
  sf::FloatRect bounds = sprite.getGlobalBounds();
  float shrinkFactor = 0.8f;

  float marginX = bounds.width * shrinkFactor / 2.f;
  float marginY = bounds.height * shrinkFactor / 2.f;

  return sf::FloatRect(
      bounds.left + marginX,
      bounds.top + marginY,
      bounds.width - marginX * 2,
      bounds.height - marginY * 2
  );
}
