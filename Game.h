#pragma once
#include "Bird.h"
#include <SFML/Graphics.hpp>

void render_bird_game(sf::RenderWindow &ventana, int level, int width,
                      int heigh, BirdType birdType = BirdType::DEFAULT);
