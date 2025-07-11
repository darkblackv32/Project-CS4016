#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "Constants.h"
#include <cmath>

namespace Physics {
    std::vector<sf::Vector2f> calcularTrayectoria(sf::Vector2f inicio, 
                                                sf::Vector2f velocidad,
                                                float airDensity = 0.001f,
                                                float crossArea = 0.05f);
    
    void applyAirResistance(sf::Vector2f& velocidad, 
                          float deltaTime,
                          float airDensity = 0.001f,
                          float crossArea = 0.05f);

    void handleGroundCollision(sf::CircleShape& figura, 
                             sf::Vector2f& velocidad, 
                             float groundLevel);
}
