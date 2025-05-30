//
// Created by jolly on 27.04.2025.
//

#include "EnvironmentManager.h"
#include <cstdlib>
#include <cmath>

EnvironmentManager::EnvironmentManager(sf::Vector2u windowSize)
    : windowSize(windowSize)
{
    treeTexture.loadFromFile("assets/tree.png");
    treeTexture2.loadFromFile("assets/tree2.png");
    treeTexture3.loadFromFile("assets/tree3.png");

    rockTexture.loadFromFile("assets/rock.png");
    rockTexture2.loadFromFile("assets/rock2.png");

    stickTexture.loadFromFile("assets/stick.png");
    stickTexture2.loadFromFile("assets/stick2.png");

    fountainTexture.loadFromFile("assets/fountain.png");
    fountainSprite.setTexture(fountainTexture);
    fountainSprite.setScale(5.0f, 5.0f);

}

void EnvironmentManager::update(const sf::Vector2f& playerPosition) {
  constexpr int chunkSize = 2048;

    if (!fountainPlaced) {
        fountainSprite.setPosition(playerPosition.x - fountainSprite.getGlobalBounds().width / 2.f,
                                   playerPosition.y - fountainSprite.getGlobalBounds().height / 2.f);
        fountainPlaced = true;
    }

    int playerChunkX = static_cast<int>(playerPosition.x) / chunkSize;
    int playerChunkY = static_cast<int>(playerPosition.y) / chunkSize;

    float visibleRadius = std::sqrt(windowSize.x * windowSize.x + windowSize.y * windowSize.y) / 2.f + 300.f;

    for (int dx = -1; dx <= 1; ++dx) {
        for (int dy = -1; dy <= 1; ++dy) {
            int chunkX = playerChunkX + dx;
            int chunkY = playerChunkY + dy;
            std::pair<int, int> chunkCoords = {chunkX, chunkY};

            if (chunks.find(chunkCoords) == chunks.end()) {
                std::vector<EnvironmentObjects> objects;

                int objectsCount = 2 + rand() % 6;
                for (int i = 0; i < objectsCount; ++i) {
                    int objectType = rand() % 3;
                    sf::Sprite sprite;

                    if (objectType == 0) {
                        int variant = rand() % 3;
                        if (variant == 0)
                            sprite.setTexture(treeTexture);
                        else if (variant == 1)
                            sprite.setTexture(treeTexture2);
                        else
                            sprite.setTexture(treeTexture3);

                        sprite.setScale(6.0f, 6.0f);

                    } else if (objectType == 1) { // камень
                        sprite.setTexture(rand() % 2 == 0 ? rockTexture : rockTexture2);
                        sprite.setScale(3.5f, 3.5f);

                    } else { // палка
                        sprite.setTexture(rand() % 2 == 0 ? stickTexture : stickTexture2);
                        sprite.setScale(2.8f, 2.8f);
                    }


                    float x = chunkX * chunkSize + rand() % chunkSize;
                    float y = chunkY * chunkSize + rand() % chunkSize;
                    sf::Vector2f position(x, y);

                    float dx = position.x - playerPosition.x;
                    float dy = position.y - playerPosition.y;
                    float distanceSquared = dx * dx + dy * dy;

                    if (distanceSquared < visibleRadius * visibleRadius) {
                        continue;
                    }

                    sprite.setPosition(x, y);
                    objects.emplace_back(sprite);
                }

                chunks[chunkCoords] = objects;
            }
        }
    }
}

void EnvironmentManager::draw(sf::RenderWindow& window) {
    for (auto& pair : chunks) {
        for (auto& obj : pair.second) {
            window.draw(obj.getSprite());
        }
    }
    window.draw(fountainSprite);
}




