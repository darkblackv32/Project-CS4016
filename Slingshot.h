#pragma once
#include <SFML/Graphics.hpp>
#include "Constants.h"

struct Slingshot {
    sf::RectangleShape posteIzq;
    sf::RectangleShape posteDer;
    sf::VertexArray bandaIzq;
    sf::VertexArray bandaDer;

    Slingshot();
    void updateBands(const sf::Vector2f& birdPosition);
};