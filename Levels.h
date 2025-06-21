#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <utility>
#include <vector>

struct Level {
  std::vector<sf::RectangleShape> objects;
  std::vector<sf::RectangleShape> targets;
  std::vector<sf::RectangleShape> floor;

  float START_LEVEL_X;
  float START_LEVEL_Y;

  int x_bound;
  int y_bound;

  Level();
  ~Level();

  void setObjects(std::vector<sf::Vector2f> &objectSizes,
                  std::vector<std::pair<float, float>> &objectPos,
                  std::vector<sf::Color> &objectColors);

  void setTargets(std::vector<sf::Vector2f> &objectSizes,
                  std::vector<std::pair<float, float>> &objectPos,
                  std::vector<sf::Color> &objectColors);
  void setFloor(std::vector<sf::Vector2f> &objectSizes,
                std::vector<std::pair<float, float>> &objectPos,
                std::vector<sf::Color> &objectColors);

  void setStarts(float x, float y);

  void setBounds(float x, float y);

  void render(sf::RenderWindow &ventana);
};

Level *return_level(int level, int width, int height);

void test_level(int level);
