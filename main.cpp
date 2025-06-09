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
  
  // Start with title screen
  int gameState = 0; // 0 = title screen, 1 = game
  BirdType currentBirdType = BirdType::DEFAULT;
  
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
      }
    } else if (gameState == 1) {
      render_bird_game(ventana, 0, width, height, currentBirdType);
    }
  }

  return 0;
}