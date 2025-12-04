#ifndef HELPER_H_
#define HELPER_H_

#include "Constants.h"
#include "PhysicsWrapper.h"
#include <SFML/Graphics.hpp>

// Convert SFML position (pixels) to Box2D position (meters)
b2Vec2 pixelsToMeters(const sf::Vector2f &pixels);
// Convert Box2D position (meters) to SFML position (pixels)
sf::Vector2f metersToPixels(const b2Vec2 &meters);
#endif // HELPER_H_
