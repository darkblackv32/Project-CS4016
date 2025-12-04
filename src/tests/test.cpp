#include "Pause.h"
#include <iostream>

int main() {
  int width = 800;
  int height = 600;
  sf::RenderWindow ventana(sf::VideoMode(width, height),
                           "Angry Birds Ultimate");
  ventana.setFramerateLimit(60);
  render_pause_menu(ventana);
  return 0;
}
