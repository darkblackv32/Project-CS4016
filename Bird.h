#pragma once
#include <SFML/Graphics.hpp>
#include "Constants.h"

struct Bird {
    sf::CircleShape figura;
    sf::Vector2f velocidad;
    bool lanzado = false;
    bool enResortera = true;

    Bird();
    void updatePhysics(float deltaTime);
    void reset();
};