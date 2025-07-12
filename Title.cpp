#include "Title.h"
#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream> // Include for std::cout

// Function to update the positions and sizes of all UI elements
// This function will be called initially and whenever the window is resized
void updateTitleMenuLayout(sf::RenderWindow &ventana,
                           sf::Sprite &background_sprite, sf::Text &gameTitle,
                           sf::RectangleShape &startButton,
                           sf::Text &startButtonText,
                           sf::RectangleShape &menuButton,
                           sf::Text &menuButtonText,
                           sf::RectangleShape &quitButton,
                           sf::Text &quitButtonText, float &titleInitialY) {

  // Update background sprite scale to fill the window
  background_sprite.setScale(
      (float)ventana.getSize().x / background_sprite.getTexture()->getSize().x,
      (float)ventana.getSize().y / background_sprite.getTexture()->getSize().y);

  // Recalculate game title position
  gameTitle.setCharacterSize(static_cast<unsigned int>(
      ventana.getSize().y / 8.0f)); // Scale font size with window height
  gameTitle.setOrigin(gameTitle.getLocalBounds().width / 2.0f,
                      gameTitle.getLocalBounds().height / 2.0f);
  gameTitle.setPosition(ventana.getSize().x / 2.0f, ventana.getSize().y / 4.0f);
  titleInitialY = gameTitle.getPosition().y; // Update initial Y for animation

  // Define common button properties relative to window size
  float buttonWidth =
      ventana.getSize().x / 4.0f; // Example: 1/4th of window width
  float buttonHeight =
      ventana.getSize().y / 12.0f; // Example: 1/12th of window height
  float verticalSpacing =
      ventana.getSize().y /
      20.0f; // Space between buttons, relative to window height

  // Recalculate button positions
  // Start button
  startButton.setSize(sf::Vector2f(buttonWidth, buttonHeight));
  startButton.setOrigin(startButton.getLocalBounds().width / 2.0f,
                        startButton.getLocalBounds().height / 2.0f);
  startButton.setPosition(ventana.getSize().x / 2.0f,
                          ventana.getSize().y /
                              2.0f); // Center vertically in the lower half

  startButtonText.setCharacterSize(static_cast<unsigned int>(
      buttonHeight * 0.6f)); // Text size relative to button height
  startButtonText.setOrigin(startButtonText.getLocalBounds().width / 2.0f,
                            startButtonText.getLocalBounds().height / 2.0f);
  startButtonText.setPosition(startButton.getPosition());

  // Menu button
  menuButton.setSize(sf::Vector2f(buttonWidth, buttonHeight));
  menuButton.setOrigin(menuButton.getLocalBounds().width / 2.0f,
                       menuButton.getLocalBounds().height / 2.0f);
  menuButton.setPosition(ventana.getSize().x / 2.0f,
                         startButton.getPosition().y + buttonHeight / 2.0f +
                             verticalSpacing + buttonHeight / 2.0f);

  menuButtonText.setCharacterSize(
      static_cast<unsigned int>(buttonHeight * 0.6f));
  menuButtonText.setOrigin(menuButtonText.getLocalBounds().width / 2.0f,
                           menuButtonText.getLocalBounds().height / 2.0f);
  menuButtonText.setPosition(menuButton.getPosition());

  // Quit button
  quitButton.setSize(sf::Vector2f(buttonWidth, buttonHeight));
  quitButton.setOrigin(quitButton.getLocalBounds().width / 2.0f,
                       quitButton.getLocalBounds().height / 2.0f);
  quitButton.setPosition(ventana.getSize().x / 2.0f,
                         menuButton.getPosition().y + buttonHeight / 2.0f +
                             verticalSpacing + buttonHeight / 2.0f);

  quitButtonText.setCharacterSize(
      static_cast<unsigned int>(buttonHeight * 0.6f));
  quitButtonText.setOrigin(quitButtonText.getLocalBounds().width / 2.0f,
                           quitButtonText.getLocalBounds().height / 2.0f);
  quitButtonText.setPosition(quitButton.getPosition());
}

int render_title(sf::RenderWindow &ventana) {

  sf::Texture background_texture;
  if (!background_texture.loadFromFile("assets/textures/portada_clean.jpg")) {
    std::cerr << "Error loading background texture." << std::endl;
    return -1;
  }
  sf::Sprite background_sprite(background_texture);

  sf::Font font; // load font
  if (!font.loadFromFile("./assets/fonts/angrybirds-regular.ttf")) {
    std::cerr << "Couldn't load font." << std::endl;
    return -1;
  }

  // blueish color
  auto button_color = sf::Color(100, 100, 250);

  // game title
  sf::Text gameTitle;
  gameTitle.setFont(font);
  gameTitle.setString("Angry politician");
  gameTitle.setFillColor(sf::Color::White);

  // start button
  sf::RectangleShape startButton;
  startButton.setFillColor(button_color);

  sf::Text startButtonText;
  startButtonText.setFont(font);
  startButtonText.setString("Start Game");
  startButtonText.setFillColor(sf::Color::White);

  // menu button
  sf::RectangleShape menuButton;
  menuButton.setFillColor(button_color);

  sf::Text menuButtonText;
  menuButtonText.setFont(font);
  menuButtonText.setString("Level selection");
  menuButtonText.setFillColor(sf::Color::White);

  // quit button
  sf::RectangleShape quitButton;
  quitButton.setFillColor(button_color);

  sf::Text quitButtonText;
  quitButtonText.setFont(font);
  quitButtonText.setString("Quit");
  quitButtonText.setFillColor(sf::Color::White);

  // clock for title animation
  sf::Clock clock;
  float titleInitialY = 0.0f;       // Will be set by updateTitleMenuLayout
  float animationAmplitude = 20.0f; // how much the title moves up and down
  float animationSpeed = 2.0f;      // how fast the title moves

  // Initial layout update
  updateTitleMenuLayout(ventana, background_sprite, gameTitle, startButton,
                        startButtonText, menuButton, menuButtonText, quitButton,
                        quitButtonText, titleInitialY);

  while (ventana.isOpen()) {
    sf::Event event;
    while (ventana.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        ventana.close();
      }
      // Handle window resize event
      if (event.type == sf::Event::Resized) {
        // Update the view to match the new window size
        sf::FloatRect visibleArea(0, 0, static_cast<float>(event.size.width),
                                  static_cast<float>(event.size.height));
        ventana.setView(sf::View(visibleArea));

        // Recalculate and update the layout of all UI elements
        updateTitleMenuLayout(ventana, background_sprite, gameTitle,
                              startButton, startButtonText, menuButton,
                              menuButtonText, quitButton, quitButtonText,
                              titleInitialY);
      }
      if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Left) {
          // Mouse coordinates are already in pixel coordinates relative to the
          // window since we are using the default view (or a view matching
          // window size)
          sf::Vector2f mousePos(static_cast<float>(event.mouseButton.x),
                                static_cast<float>(event.mouseButton.y));

          if (startButton.getGlobalBounds().contains(mousePos)) {
            return 1; // Start the game
          } else if (menuButton.getGlobalBounds().contains(mousePos)) {
            return 2; // Level selection
          } else if (quitButton.getGlobalBounds().contains(mousePos)) {
            return 3; // Quit
          }
        }
      }
    }

    // title animation
    float time = clock.getElapsedTime().asSeconds();
    float newY =
        titleInitialY + std::sin(time * animationSpeed) * animationAmplitude;
    gameTitle.setPosition(gameTitle.getPosition().x, newY);

    ventana.clear();
    ventana.draw(background_sprite);
    ventana.draw(gameTitle);
    ventana.draw(startButton);
    ventana.draw(startButtonText);
    ventana.draw(menuButton);
    ventana.draw(menuButtonText);
    ventana.draw(quitButton);
    ventana.draw(quitButtonText);
    ventana.display();
  }
  return 0;
}
