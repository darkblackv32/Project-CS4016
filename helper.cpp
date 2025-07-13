#include "helper.h"
#include <SFML/Graphics.hpp>

// Convert SFML position (pixels) to Box2D position (meters)
b2Vec2 pixelsToMeters(const sf::Vector2f &pixels) {
  return b2Vec2(pixels.x / SCALE, pixels.y / SCALE);
}

// Convert Box2D position (meters) to SFML position (pixels)
sf::Vector2f metersToPixels(const b2Vec2 &meters) {
  return sf::Vector2f(meters.x * SCALE, meters.y * SCALE);
}
