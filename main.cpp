#include "Game.h"
#include "Menu.h"
#include "Title.h"
#include <SFML/Graphics.hpp>
#include <cmath>

int main() {
  int width = 800;
  int height = 600;
  sf::RenderWindow ventana(sf::VideoMode(width, height),
                           "Angry Birds Ultimate");
  ventana.setFramerateLimit(120);

  // Start with title screen
  int gameState = 0; // 0 = title screen, 1 = game
  BirdType currentBirdType = BirdType::DEFAULT;

  // selected level
  int level = 0;

  while (ventana.isOpen()) {
    sf::Event evento;
    while (ventana.pollEvent(evento)) {
      if (evento.type == sf::Event::Closed)
        ventana.close();
    }

    if (gameState == 0) {
      int next = render_title(ventana);

      if (next == -1) {
        return 1;
      } else if (next == 1) {
        gameState = 1;
      } else if (next == 2) {
        gameState = 2;
      } else if (next == 3) {
        ventana.close();
      }
    } else if (gameState == 1) {
      int response =
          render_bird_game(ventana, level, width, height, currentBirdType);
      if (response == 2) {
        gameState = 0;
      } else if (response == 3) {
        ventana.close();
      }
    } else if (gameState == 2) {
      level = render_menu(ventana);
      if (level > -1) {
        gameState = 1;
      }
    }
  }

  return 0;
}
