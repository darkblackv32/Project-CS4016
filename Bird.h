#pragma once
#include "Constants.h"
#include <SFML/Graphics.hpp>
#include <string>

enum class BirdType {
    DEFAULT,
    MILEI,
    FUJIMORI
};

struct Bird {
  sf::CircleShape figura;  // Used for collision and physics
  sf::Sprite sprite;   // Used for drawing custom images
  sf::Vector2f velocidad;
  bool lanzado = false;
  bool enResortera = true;
  sf::Texture texture;
  sf::Texture idleTexture;  //(1.png)
  sf::Texture flyingTexture; //  (2.png)
  BirdType type;
  bool useSprite = false;

  Bird(BirdType birdType = BirdType::DEFAULT);

  void createDefaultTexture();
  void updatePhysics(float deltaTime);
  void reset();
  void loadTextures();
  void updateTextureState();
  void setBirdType(BirdType birdType);
  BirdType getBirdType() const;
  void draw(sf::RenderWindow& window);
};