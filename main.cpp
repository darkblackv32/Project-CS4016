#include "Game.h"
#include "Menu.h"
#include "Title.h"
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <cmath>

int main() {
  int width = sf::VideoMode::getDesktopMode().width;
  int height = sf::VideoMode::getDesktopMode().height;
  sf::RenderWindow ventana(sf::VideoMode(width, height),
                           "Angry Birds Ultimate");
  ventana.setFramerateLimit(60);

  sf::Music music;
  if (!music.openFromFile("assets/audios/milei_song.ogg")) {
    return -1; // error
  }
  music.setLoop(true);

  // Start with title screen
  int gameState = 0; // 0 = title screen, 1 = game
  BirdType currentBirdType = BirdType::MILEI;

  // selected level
  int level = 0;

  while (ventana.isOpen()) {
    sf::Event evento;
    while (ventana.pollEvent(evento)) {
      if (evento.type == sf::Event::Closed)
        ventana.close();
    }

    if (gameState == 0) {
      if (music.getStatus() != sf::Music::Playing) {
        music.play();
      }
      int next = render_title(ventana);

      if (next == 1) { // start game
        music.stop();
        gameState = 1;
      } else if (next == 2) { // level selection
        gameState = 2;
      } else if (next == -1) { // quit
        ventana.close();
      }
    } else if (gameState == 1) {
      music.stop();
      int response =
          render_bird_game(ventana, level, width, height, currentBirdType);
      if (response == 2) {
        gameState = 0;
      } else if (response == 3) {
        ventana.close();
      }
    } else if (gameState == 2) {
      int selected_level = render_menu(ventana);
      if (selected_level != -1) {
        level = selected_level;
        gameState = 1;
      } else {
        gameState = 0;
      }
    }
  }

  return 0;
}
