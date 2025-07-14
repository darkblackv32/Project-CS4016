#pragma once
#include "Constants.h"
#include <SFML/Graphics.hpp>
#include <memory>
#include <string>
#include <unordered_map>

enum class BirdType { MILEI, FUJIMORI, KENJI, MONTESINOS, GARCIA, ACUNA, CASTILLO };

struct BirdTextureCache; // forward declaration for texture cache

struct Bird {
  sf::CircleShape figura; // collision and physics
  sf::Sprite sprite;      // rendering
  sf::Vector2f velocidad;
  bool lanzado = false;
  bool enResortera = true;
  BirdType type;
  sf::Vector2f pos_resortera;

  // textures (managed via cache)
  sf::Texture *idleTexture = nullptr;
  sf::Texture *flyingTexture = nullptr;

  Bird(BirdType birdType = BirdType::MILEI,
       sf::Vector2f pr = sf::Vector2f(100.0f, 470.0f));
  void reset();
  void loadTextures();
  void updateTextureState();
  void setBirdType(BirdType birdType);
  BirdType getBirdType() const;
  void draw(sf::RenderWindow &window);
};
struct BirdTextureCache { // texture cache implementation
  static std::unordered_map<BirdType, std::pair<std::unique_ptr<sf::Texture>,
                                                std::unique_ptr<sf::Texture>>>
      cache;

};
