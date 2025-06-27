#include "helper.h"
#include "polyphysics.h"
#include <SFML/Graphics.hpp>

// --- FUNCIONES DE AYUDA ---
sf::Vector2f toSfmlVec(const Vec2 &v) { return sf::Vector2f(v.x, v.y); }
Vec2 fromSfmlVec(const sf::Vector2f &v) { return Vec2(v.x, v.y); }
