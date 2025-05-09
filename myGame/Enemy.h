#ifndef ENEMY_H
#define ENEMY_H
class Player;

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

class Enemy {
private:
    static sf::Texture enemyTexture;
    sf::Sprite enemySprite;
    float speed = 1;
    sf::Vector2f direction;
    sf::Clock movementClock;
    float changeDirection;
    int health;
    int damage = 1;

public:
    Enemy();
    static void loadTexture();
    void update(float deltaTime, const sf::Vector2f &playerPosition,
                Player &player, std::vector<Enemy> &enemies);
    sf::Vector2f getPosition() const;
    void draw(sf::RenderWindow& window) const;
    void randomizeDirection();
    void setPosition(float x, float y) { enemySprite.setPosition(x, y); }
    void takeDamage();
    bool isAlive() const;
    sf::FloatRect getBounds() const;
    sf::Clock attackCooldown;
    sf::Time attackDelay = sf::seconds(1.f);
};


#endif //ENEMY_H
