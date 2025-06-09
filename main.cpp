#include "Game.h"
#include "Title.h"
#include <SFML/Graphics.hpp>
#include <cmath>

int main() {
  int width = 800;
  int height = 600;
  sf::RenderWindow ventana(sf::VideoMode(width, height),
                           "Angry Birds Ultimate");
  ventana.setFramerateLimit(60);

  while (ventana.isOpen()) {
    sf::Event evento;
    while (ventana.pollEvent(evento)) {
      if (evento.type == sf::Event::Closed)
        ventana.close();
    }
    int next = render_title(ventana);

    if (next == -1) {
      // The text font couldn't load
      return 1;
    } else if (next == 1) {
      // Load game
      render_bird_game(ventana, 0, width, height);
    }
  }

  return 0;
}
