#pragma once
#include <SFML/Graphics.hpp>
#include "Bird.h"

void render_bird_game(sf::RenderWindow &ventana, int level, int width, int height, BirdType birdType = BirdType::DEFAULT);