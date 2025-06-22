#pragma once
#include "Bird.h"
#include <SFML/Graphics.hpp>

int render_bird_game(sf::RenderWindow &ventana, int level, int width,
                     int height, BirdType birdType = BirdType::DEFAULT);
