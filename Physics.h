#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "Constants.h"

namespace Physics {
    std::vector<sf::Vector2f> calcularTrayectoria(sf::Vector2f inicio, sf::Vector2f velocidad);
    
    void handleGroundCollision(sf::CircleShape& figura, 
                              sf::Vector2f& velocidad, 
                              float groundLevel);
}