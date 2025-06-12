#include "Levels.h"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <iostream>
#include <utility>
#include <vector>

int WIDTH = 800;
int HEIGHT = 600;

float BLOCK = 30;

Level::Level() {}
Level::~Level() {}

void Level::setObjects(std::vector<sf::Vector2f> &objectSizes,
                       std::vector<std::pair<float, float>> &objectPos,
                       std::vector<sf::Color> &objectColors) {

  std::vector<sf::RectangleShape> objects;
  for (int i = 0; i < objectSizes.size(); i++) {
    sf::RectangleShape temp(objectSizes[i]);
    temp.setPosition(objectPos[i].first, objectPos[i].second);
    temp.setFillColor(objectColors[i]);
    temp.setOutlineColor({170, 143, 24});
    temp.setOutlineThickness(1.0f);

    objects.push_back(temp);
  }

  this->objects = objects;
}

void Level::setTargets(std::vector<sf::Vector2f> &objectSizes,
                       std::vector<std::pair<float, float>> &objectPos,
                       std::vector<sf::Color> &objectColors) {

  std::vector<sf::RectangleShape> objects;
  for (int i = 0; i < objectSizes.size(); i++) {
    sf::RectangleShape temp(objectSizes[i]);
    temp.setPosition(objectPos[i].first, objectPos[i].second);
    temp.setFillColor(objectColors[i]);

    objects.push_back(temp);
  }

  this->targets = objects;
}

void Level::setFloor(std::vector<sf::Vector2f> &objectSizes,
                     std::vector<std::pair<float, float>> &objectPos,
                     std::vector<sf::Color> &objectColors) {

  std::vector<sf::RectangleShape> objects;
  for (int i = 0; i < objectSizes.size(); i++) {
    sf::RectangleShape temp(objectSizes[i]);
    temp.setPosition(objectPos[i].first, objectPos[i].second);
    temp.setFillColor(objectColors[i]);

    objects.push_back(temp);
  }

  this->floor = objects;
}

void Level::setStarts(float x, float y) {
  this->START_LEVEL_X = x;
  this->START_LEVEL_Y = y;
}

void Level::setBounds(float x, float y) {
  this->x_bound = x;
  this->y_bound = y;
}

void Level::render(sf::RenderWindow &ventana) {
  for (int i = 0; i < this->floor.size(); i++) {
    ventana.draw(this->floor[i]);
  }
  for (int i = 0; i < this->objects.size(); i++) {
    ventana.draw(this->objects[i]);
  }
  for (int i = 0; i < this->targets.size(); i++) {
    ventana.draw(this->targets[i]);
  }
}

Level *return_level(int level, int width, int height) {
  std::vector<sf::Vector2f> objSizes;
  std::vector<std::pair<float, float>> objPositions;
  std::vector<sf::Color> objColors;
  Level *l = new Level();
  float START_LEVEL_X;
  float START_LEVEL_Y;
  int bound_x;
  int bound_y;

  switch (level) {
  case 0:
    bound_x = 1000;
    bound_y = 700;
    START_LEVEL_X = bound_x - BLOCK * 17;
    START_LEVEL_Y = bound_y - BLOCK * 3;

    objSizes = {
        sf::Vector2f(BLOCK, 2 * BLOCK), sf::Vector2f(6 * BLOCK, BLOCK),
        sf::Vector2f(6 * BLOCK, BLOCK), sf::Vector2f(BLOCK, 2 * BLOCK),
        sf::Vector2f(BLOCK, 7 * BLOCK), sf::Vector2f(BLOCK, 7 * BLOCK),
        sf::Vector2f(BLOCK * 6, BLOCK), sf::Vector2f(BLOCK, 7 * BLOCK),
        sf::Vector2f(BLOCK, 7 * BLOCK), sf::Vector2f(4 * BLOCK, BLOCK),
    };

    objPositions = {
        std::make_pair(START_LEVEL_X, START_LEVEL_Y - 2 * BLOCK),
        std::make_pair(START_LEVEL_X, START_LEVEL_Y),
        std::make_pair(START_LEVEL_X + 6 * BLOCK, START_LEVEL_Y),
        std::make_pair(START_LEVEL_X + 11 * BLOCK, START_LEVEL_Y - 2 * BLOCK),
        std::make_pair(START_LEVEL_X + 3 * BLOCK, START_LEVEL_Y - 7 * BLOCK),
        std::make_pair(START_LEVEL_X + 8 * BLOCK, START_LEVEL_Y - 7 * BLOCK),
        std::make_pair(START_LEVEL_X + 3 * BLOCK, START_LEVEL_Y - 8 * BLOCK),
        std::make_pair(START_LEVEL_X + 4 * BLOCK, START_LEVEL_Y - 15 * BLOCK),
        std::make_pair(START_LEVEL_X + 7 * BLOCK, START_LEVEL_Y - 15 * BLOCK),
        std::make_pair(START_LEVEL_X + 4 * BLOCK, START_LEVEL_Y - 16 * BLOCK),
    };

    objColors = {
        {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0},
        {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0},
    };

    l->setObjects(objSizes, objPositions, objColors);

    objSizes = {sf::Vector2f(bound_x, BLOCK * 2)};
    objPositions = {std::make_pair(0.0f, bound_y - 2 * BLOCK)};
    objColors = {{120, 110, 100}};

    l->setFloor(objSizes, objPositions, objColors);

    objSizes = {sf::Vector2f(BLOCK, BLOCK)};
    objPositions = {
        std::make_pair(START_LEVEL_X + 5.5 * BLOCK, START_LEVEL_Y - BLOCK)};
    objColors = {{9, 186, 45}};

    l->setTargets(objSizes, objPositions, objColors);

    l->setStarts(START_LEVEL_X, START_LEVEL_Y);

    l->setBounds(bound_x, bound_y);

    break;
  }

  return l;
}

void test_level(int level) {
  sf::RenderWindow ventana(sf::VideoMode(WIDTH, HEIGHT),
                           "Angry Birds Ultimate");
  ventana.setFramerateLimit(60);

  sf::RectangleShape fondo({800.0f, 600.0f});
  fondo.setFillColor({135, 206, 235});

  Level *lev = return_level(0, WIDTH, HEIGHT);

  while (ventana.isOpen()) {
    sf::Event evento;
    while (ventana.pollEvent(evento)) {
      if (evento.type == sf::Event::Closed)
        ventana.close();
    }

    ventana.clear(sf::Color(50, 50, 150)); // Dark blue background
    ventana.draw(fondo);
    lev->render(ventana);
    ventana.display();
  }
}
