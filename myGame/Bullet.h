#pragma once
#include <SFML/Graphics.hpp>

class Bullet {
public:
  Bullet(sf::Vector2f startPos, sf::Vector2f targetPos);

  void update();
  void draw(sf::RenderWindow& window) const;
  sf::FloatRect getBounds() const;

  bool isActive;

private:
  sf::Sprite sprite;
  static sf::Texture bulletTexture;
  static float speed;

  sf::Vector2f velocity;
  sf::Vector2f startPosition;
  sf::Vector2f targetPosition;
  float maxDistance;
};
