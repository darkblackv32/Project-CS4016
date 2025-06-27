#ifndef HELPER_H_
#define HELPER_H_

#include "polyphysics.h"
#include <SFML/Graphics.hpp>

sf::Vector2f toSfmlVec(const Vec2 &v);
Vec2 fromSfmlVec(const sf::Vector2f &v);

#endif // HELPER_H_
