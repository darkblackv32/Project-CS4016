#include "Pause.h"
#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>

int render_pause_menu(sf::RenderWindow &ventana) {

  sf::Font font; // load font

  ventana.setView(ventana.getDefaultView());

  // change font
  if (!font.loadFromFile("./assets/fonts/angrybirds-regular.ttf")) {
    std::cout << "Couldn't load font" << std::endl;
  }

  // crimsom color
  auto button_color = sf::Color(220, 20, 60);

  // box for options
  sf::RectangleShape box(
      sf::Vector2f(ventana.getSize().x / 3, ventana.getSize().y / 3 * 1.5));
  box.setFillColor(sf::Color(100, 100, 250)); // blueish color
  box.setOrigin(box.getLocalBounds().width / 2,
                box.getLocalBounds().height / 2);
  box.setPosition(ventana.getSize().x / 2, ventana.getSize().y / 2);

  // game title
  sf::Text gameTitle;
  gameTitle.setFont(font);
  gameTitle.setString("Pausa");
  gameTitle.setCharacterSize(60);
  gameTitle.setFillColor(sf::Color::White);
  gameTitle.setOrigin(gameTitle.getLocalBounds().width / 2,
                      gameTitle.getLocalBounds().height / 2);
  gameTitle.setPosition(ventana.getSize().x / 2,
                        box.getPosition().y - box.getSize().y / 2 + 40);

  // Resume button
  sf::RectangleShape resumeButton(
      sf::Vector2f(box.getLocalBounds().width / 2, 70));
  resumeButton.setFillColor(button_color);
  resumeButton.setOrigin(resumeButton.getLocalBounds().width / 2,
                         resumeButton.getLocalBounds().height / 2);
  resumeButton.setPosition(ventana.getSize().x / 2,
                           gameTitle.getPosition().y + 75);

  sf::Text resumeButtonText;
  resumeButtonText.setFont(font);
  resumeButtonText.setString("Resume");
  resumeButtonText.setCharacterSize(36);
  resumeButtonText.setFillColor(sf::Color::White);
  resumeButtonText.setOrigin(resumeButtonText.getLocalBounds().width / 2,
                             resumeButtonText.getLocalBounds().height / 2);
  resumeButtonText.setPosition(resumeButton.getPosition());

  // return button
  sf::RectangleShape returnButton(
      sf::Vector2f(box.getLocalBounds().width / 2, 70));
  returnButton.setFillColor(button_color);
  returnButton.setOrigin(returnButton.getLocalBounds().width / 2,
                         returnButton.getLocalBounds().height / 2);
  returnButton.setPosition(ventana.getSize().x / 2,
                           resumeButton.getPosition().y + 75);

  sf::Text returnButtonText;
  returnButtonText.setFont(font);
  returnButtonText.setString("Title Screen");
  returnButtonText.setCharacterSize(36);
  returnButtonText.setFillColor(sf::Color::White);
  returnButtonText.setOrigin(returnButtonText.getLocalBounds().width / 2,
                             returnButtonText.getLocalBounds().height / 2);
  returnButtonText.setPosition(returnButton.getPosition());

  // quit button
  sf::RectangleShape quitButton(
      sf::Vector2f(box.getLocalBounds().width / 2, 70));
  quitButton.setFillColor(button_color);
  quitButton.setOrigin(quitButton.getLocalBounds().width / 2,
                       quitButton.getLocalBounds().height / 2);
  quitButton.setPosition(ventana.getSize().x / 2,
                         returnButton.getPosition().y + 75);

  sf::Text quitButtonText;
  quitButtonText.setFont(font);
  quitButtonText.setString("Quit Game");
  quitButtonText.setCharacterSize(36);
  quitButtonText.setFillColor(sf::Color::White);
  quitButtonText.setOrigin(quitButtonText.getLocalBounds().width / 2,
                           quitButtonText.getLocalBounds().height / 2);
  quitButtonText.setPosition(quitButton.getPosition());

  while (ventana.isOpen()) {
    sf::Event event;
    while (ventana.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        ventana.close();
      }
      if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Left) {
          // check if the mouse click is within the button bounds
          if (resumeButton.getGlobalBounds().contains(
                  static_cast<float>(event.mouseButton.x),
                  static_cast<float>(event.mouseButton.y))) {
            // resume the game
            // continue the game
            return 1;
          } else if (returnButton.getGlobalBounds().contains(
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

    ventana.clear(sf::Color(50, 50, 150));
    ventana.draw(box);
    ventana.draw(gameTitle);
    ventana.draw(resumeButton);
    ventana.draw(resumeButtonText);
    ventana.draw(returnButton);
    ventana.draw(returnButtonText);
    ventana.draw(quitButton);
    ventana.draw(quitButtonText);
    ventana.display();
  }

  return 1;
}
