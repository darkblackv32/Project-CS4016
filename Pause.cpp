#include "Pause.h"
#include <SFML/Graphics.hpp>
#include <iostream>

int render_pause_menu(sf::RenderWindow &ventana, sf::RectangleShape &fondo,
                      Level *lev, Slingshot &resortera, Bird &pajaro,
                      std::vector<Bird> &birdQueue) {

  sf::Font font; // load font

  // Get the current view's properties. This is crucial for positioning
  // elements relative to the view, not just the window's pixel size.
  sf::View currentView = ventana.getView();
  sf::Vector2f viewCenter = currentView.getCenter();
  sf::Vector2f viewSize = currentView.getSize();

  // Load font
  if (!font.loadFromFile("./assets/fonts/angrybirds-regular.ttf")) {
    std::cout << "Couldn't load font" << std::endl;
  }

  float verticalPadding = 20.0f; // Space between buttons

  // Crimson color for buttons
  auto button_color = sf::Color(220, 20, 60, 180);

  // Box for options - size relative to view, positioned at view center
  // Using viewSize for relative sizing ensures it scales with the view
  sf::RectangleShape box(
      sf::Vector2f(viewSize.x / 3.0f, viewSize.y / 3.0f * 1.5f));
  box.setFillColor(
      sf::Color(100, 100, 250, 180)); // blueish color, semi-transparent
  box.setOrigin(box.getLocalBounds().width / 2.0f,
                box.getLocalBounds().height / 2.0f); // Set origin to its center
  box.setPosition(viewCenter.x,
                  viewCenter.y); // Position at the center of the view

  // Game title
  sf::Text gameTitle;
  gameTitle.setFont(font);
  gameTitle.setString("Pause");
  gameTitle.setCharacterSize(static_cast<unsigned int>(
      viewSize.y / 10.0f)); // Size relative to view height
  gameTitle.setFillColor(sf::Color::White);
  gameTitle.setOrigin(gameTitle.getLocalBounds().width / 2.0f,
                      gameTitle.getLocalBounds().height / 2.0f);

  // Define common button properties
  float buttonWidth =
      box.getLocalBounds().width * 0.7f; // Buttons take 70% of box width
  float buttonHeight =
      viewSize.y / 12.0f; // Button height relative to view height

  // Calculate total height needed for elements (title + 3 buttons + 4 paddings)
  // This is a more robust way to distribute space
  float totalElementHeight =
      gameTitle.getLocalBounds().height + (buttonHeight * 3) +
      (verticalPadding * 4); // 3 buttons, 4 gaps (top, between, bottom)

  // Calculate the starting Y position for the first element (gameTitle)
  // This will ensure all elements are collectively centered within the box
  float startY =
      box.getPosition().y - box.getLocalBounds().height / 2.0f +
      (box.getLocalBounds().height - totalElementHeight) /
          2.0f; // Start from top of box, add half the remaining space

  // Position game title
  gameTitle.setPosition(box.getPosition().x,
                        startY + gameTitle.getLocalBounds().height / 2.0f);

  // Calculate the Y position for the first button, below the title
  float currentY = gameTitle.getPosition().y +
                   gameTitle.getLocalBounds().height / 2.0f +
                   verticalPadding * 2;

  // Resume button
  sf::RectangleShape resumeButton(sf::Vector2f(buttonWidth, buttonHeight));
  resumeButton.setFillColor(button_color);
  resumeButton.setOrigin(resumeButton.getLocalBounds().width / 2.0f,
                         resumeButton.getLocalBounds().height / 2.0f);
  resumeButton.setPosition(box.getPosition().x, currentY + buttonHeight / 2.0f);

  sf::Text resumeButtonText;
  resumeButtonText.setFont(font);
  resumeButtonText.setString("Resume");
  resumeButtonText.setCharacterSize(static_cast<unsigned int>(
      buttonHeight * 0.6f)); // Text size relative to button height
  resumeButtonText.setFillColor(sf::Color::White);
  resumeButtonText.setOrigin(resumeButtonText.getLocalBounds().width / 2.0f,
                             resumeButtonText.getLocalBounds().height / 2.0f);
  resumeButtonText.setPosition(resumeButton.getPosition());

  // Update currentY for the next button
  currentY =
      resumeButton.getPosition().y + buttonHeight / 2.0f + verticalPadding;

  // return button (Title Screen)
  sf::RectangleShape returnButton(sf::Vector2f(buttonWidth, buttonHeight));
  returnButton.setFillColor(button_color);
  returnButton.setOrigin(returnButton.getLocalBounds().width / 2.0f,
                         returnButton.getLocalBounds().height / 2.0f);
  returnButton.setPosition(box.getPosition().x, currentY + buttonHeight / 2.0f);

  sf::Text returnButtonText;
  returnButtonText.setFont(font);
  returnButtonText.setString("Title Screen");
  returnButtonText.setCharacterSize(
      static_cast<unsigned int>(buttonHeight * 0.6f));
  returnButtonText.setFillColor(sf::Color::White);
  returnButtonText.setOrigin(returnButtonText.getLocalBounds().width / 2.0f,
                             returnButtonText.getLocalBounds().height / 2.0f);
  returnButtonText.setPosition(returnButton.getPosition());

  // Update currentY for the next button
  currentY =
      returnButton.getPosition().y + buttonHeight / 2.0f + verticalPadding;

  // quit button
  sf::RectangleShape quitButton(sf::Vector2f(buttonWidth, buttonHeight));
  quitButton.setFillColor(button_color);
  quitButton.setOrigin(quitButton.getLocalBounds().width / 2.0f,
                       quitButton.getLocalBounds().height / 2.0f);
  quitButton.setPosition(box.getPosition().x, currentY + buttonHeight / 2.0f);

  sf::Text quitButtonText;
  quitButtonText.setFont(font);
  quitButtonText.setString("Quit Game");
  quitButtonText.setCharacterSize(
      static_cast<unsigned int>(buttonHeight * 0.6f));
  quitButtonText.setFillColor(sf::Color::White);
  quitButtonText.setOrigin(quitButtonText.getLocalBounds().width / 2.0f,
                           quitButtonText.getLocalBounds().height / 2.0f);
  quitButtonText.setPosition(quitButton.getPosition());

  while (ventana.isOpen()) {
    sf::Event event;
    while (ventana.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        ventana.close();
      }
      if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Left) {
          // Map pixel coordinates to world coordinates using the current view
          sf::Vector2f mousePos = ventana.mapPixelToCoords(
              sf::Vector2i(event.mouseButton.x, event.mouseButton.y),
              currentView); // Pass currentView to map pixel to world coords

          if (resumeButton.getGlobalBounds().contains(mousePos)) {
            return 1; // Resume the game
          } else if (returnButton.getGlobalBounds().contains(mousePos)) {
            return 2; // Go to Title Screen
          } else if (quitButton.getGlobalBounds().contains(mousePos)) {
            return 3; // Quit Game
          }
        }
      }
    }

    ventana.clear(sf::Color::Black);

    // level stuff as background
    ventana.draw(fondo);
    lev->render(ventana);
    resortera.draw(ventana);
    pajaro.draw(ventana);

    for (const auto &bird : birdQueue) {
      ventana.draw(bird.sprite);
    }

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
