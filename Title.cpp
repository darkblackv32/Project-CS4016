#include "Title.h"
#include <SFML/Graphics.hpp>
#include <cmath>

int render_title(sf::RenderWindow &ventana) {

  sf::Font font; // load font

  // change font
  if (!font.loadFromFile("./assets/fonts/angrybirds-regular.ttf")) {
    return -1;
  }

  // blueish color
  auto button_color = sf::Color(100, 100, 250);
  int box_width = 300;

  // game title
  sf::Text gameTitle;
  gameTitle.setFont(font);
  gameTitle.setString("Angry politician");
  gameTitle.setCharacterSize(72);
  gameTitle.setFillColor(sf::Color::White);
  gameTitle.setOrigin(gameTitle.getLocalBounds().width / 2,
                      gameTitle.getLocalBounds().height / 2);
  gameTitle.setPosition(ventana.getSize().x / 2, ventana.getSize().y / 4);

  // start button
  sf::RectangleShape startButton(sf::Vector2f(box_width, 70));
  startButton.setFillColor(button_color);
  startButton.setOrigin(startButton.getLocalBounds().width / 2,
                        startButton.getLocalBounds().height / 2);
  startButton.setPosition(ventana.getSize().x / 2,
                          ventana.getSize().y / 3 + 100);

  sf::Text startButtonText;
  startButtonText.setFont(font);
  startButtonText.setString("Start Game");
  startButtonText.setCharacterSize(36);
  startButtonText.setFillColor(sf::Color::White);
  startButtonText.setOrigin(startButtonText.getLocalBounds().width / 2,
                            startButtonText.getLocalBounds().height / 2);
  startButtonText.setPosition(startButton.getPosition());

  // menu button
  sf::RectangleShape menuButton(sf::Vector2f(box_width, 70));
  menuButton.setFillColor(button_color);
  menuButton.setOrigin(menuButton.getLocalBounds().width / 2,
                       menuButton.getLocalBounds().height / 2);
  menuButton.setPosition(ventana.getSize().x / 2,
                         startButton.getPosition().y + 100 +
                             startButton.getLocalBounds().getSize().y);

  sf::Text menuButtonText;
  menuButtonText.setFont(font);
  menuButtonText.setString("Level selection");
  menuButtonText.setCharacterSize(36);
  menuButtonText.setFillColor(sf::Color::White);
  menuButtonText.setOrigin(menuButtonText.getLocalBounds().width / 2,
                           menuButtonText.getLocalBounds().height / 2);
  menuButtonText.setPosition(menuButton.getPosition());

  // quit button
  sf::RectangleShape quitButton(sf::Vector2f(box_width, 70));
  quitButton.setFillColor(button_color);
  quitButton.setOrigin(quitButton.getLocalBounds().width / 2,
                       quitButton.getLocalBounds().height / 2);
  quitButton.setPosition(ventana.getSize().x / 2,
                         menuButton.getPosition().y + 100 +
                             menuButton.getLocalBounds().getSize().y);

  sf::Text quitButtonText;
  quitButtonText.setFont(font);
  quitButtonText.setString("Quit");
  quitButtonText.setCharacterSize(36);
  quitButtonText.setFillColor(sf::Color::White);
  quitButtonText.setOrigin(quitButtonText.getLocalBounds().width / 2,
                           quitButtonText.getLocalBounds().height / 2);
  quitButtonText.setPosition(quitButton.getPosition());

  // clock for title animation
  sf::Clock clock;
  float titleInitialY = gameTitle.getPosition().y;
  float animationAmplitude = 20.0f; // how much the title moves up and down
  float animationSpeed = 2.0f;      // how fast the title moves

  while (ventana.isOpen()) {
    sf::Event event;
    while (ventana.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        ventana.close();
      }
      if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Left) {
          // check if the mouse click is within the button bounds
          if (startButton.getGlobalBounds().contains(
                  static_cast<float>(event.mouseButton.x),
                  static_cast<float>(event.mouseButton.y))) {
            // start the game
            // continue the game
            return 1;
          } else if (menuButton.getGlobalBounds().contains(
                         static_cast<float>(event.mouseButton.x),
                         static_cast<float>(event.mouseButton.y))) {
            return 2;
          } else if (quitButton.getGlobalBounds().contains(
                         static_cast<float>(event.mouseButton.x),
                         static_cast<float>(event.mouseButton.y))) {
            return 3;
          }
        }
      }
    }
    // update title position for animation
    float elapsedTime = clock.getElapsedTime().asSeconds();
    float newY = titleInitialY +
                 animationAmplitude * std::sin(elapsedTime * animationSpeed);
    gameTitle.setPosition(gameTitle.getPosition().x, newY);

    ventana.clear(sf::Color(50, 50, 150)); // dark blue background
    ventana.draw(gameTitle);
    ventana.draw(startButton);
    ventana.draw(startButtonText);
    ventana.draw(menuButton);
    ventana.draw(menuButtonText);
    ventana.draw(quitButton);
    ventana.draw(quitButtonText);
    ventana.display();
  }

  return 1;
}
