/*-*- +format-on-save-disabled-modes: (c-mode); -*- */
#pragma once
#include "Constants.h"
#include <SFML/Graphics.hpp>
#include <cmath>

class Slingshot {
private:
  sf::Texture woodTexture;
  sf::Texture rubberTexture;

public:
  sf::RectangleShape posteIzq;
  sf::RectangleShape posteDer;
  sf::VertexArray bandaIzq;
  sf::VertexArray bandaDer;

  Slingshot(sf::Vector2f &pos_resortera);
  void loadTextures();
  void updateBands(const sf::Vector2f &birdPosition, bool isStretched);
  void draw(sf::RenderWindow &window);
};
