#pragma once
#include <SFML/Graphics.hpp>

class Bullet {
public:
  Bullet(sf::Vector2f startPos, sf::Vector2f targetPos, float scale = 0.5f, int damage = 100, float maxRange = 10000.f, const std::string& texturePath = "assets/bullet.gif");

  void update();
  void draw(sf::RenderWindow& window) const;
  sf::FloatRect getBounds() const;

  bool isActive;

  int damage = 100;

private:
  sf::Sprite sprite;
  float speed = 3.f;

  sf::Vector2f velocity;
  sf::Vector2f startPosition;
  float initialMaxDistance;

  static std::map<std::string, sf::Texture> textureCache;

  static const sf::Texture& getTexture(const std::string& path);
};
