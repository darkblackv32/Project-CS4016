#include "Bird.h"
#include "Levels.h"
#include "Slingshot.h"
#include <SFML/Graphics.hpp>
#include <iostream>

int victory_screen(sf::RenderWindow &window, sf::RectangleShape &fondo,
                   Level *lev, Slingshot &resortera, Bird &pajaro,
                   std::vector<Bird> &birdQueue);
