#pragma once

#include "PhysicsWrapper.h"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Vector2.hpp>
#include <utility>
#include <vector>

struct Level {
  std::vector<sf::RectangleShape> objects;
  std::vector<sf::RectangleShape> targets;
  std::vector<sf::RectangleShape> floor;

  std::vector<b2Body *> m_bodies;
  std::vector<b2Body *> m_targets;
  std::vector<b2Body *> m_static;

  float START_LEVEL_X;
  float START_LEVEL_Y;

  int x_bound;
  int y_bound;

  PhysicsWrapper m_physics;

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

  b2Body *createBox(float x, float y, float halfWidth, float halfHeight);
  b2Body *createBird(const sf::Vector2f &pos, float radius);

  void render(sf::RenderWindow &ventana);
  void run(float deltaTime);
  int over();

  void update();
};

Level *return_level(int level, int width, int height);

void test_level(int level);
