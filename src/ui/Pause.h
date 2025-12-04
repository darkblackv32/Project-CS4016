#ifndef PAUSE_H_
#define PAUSE_H_

#include "Bird.h"
#include "Levels.h"
#include "Slingshot.h"
#include <SFML/Graphics.hpp>

int render_pause_menu(sf::RenderWindow &ventana, sf::RectangleShape &fondo,
                      Level *lev, Slingshot &resortera, Bird &pajaro,
                      std::vector<Bird> &birdQueue);

#endif // PAUSE_H_
