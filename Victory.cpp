#include "Victory.h"

int victory_screen(sf::RenderWindow &window, sf::RectangleShape &fondo,
                   Level *lev, Slingshot &resortera, Bird &pajaro) {

  sf::Font font;
  if (!font.loadFromFile("./assets/fonts/angrybirds-regular.ttf")) {
    std::cout << "Couldn't load font" << std::endl;
  }

  // background
  sf::RectangleShape backgroundRect;
  backgroundRect.setSize(sf::Vector2f(700, 100));
  backgroundRect.setFillColor(
      sf::Color(50, 50, 50, 180)); // Dark grey, semi-transparent
  backgroundRect.setOutlineThickness(5);
  backgroundRect.setOutlineColor(sf::Color::White);
  backgroundRect.setOrigin(backgroundRect.getLocalBounds().width / 2.0f,
                           backgroundRect.getLocalBounds().height / 2.0f);
  backgroundRect.setPosition(window.getSize().x / 2.0f,
                             window.getSize().y / 3.0f * 2.0f +
                                 5); // Adjusted to center around buttons

  // Congratulations Message
  sf::Text congratulationsText;
  congratulationsText.setFont(font);
  congratulationsText.setString("Congratulations! You won!");
  congratulationsText.setCharacterSize(60);
  congratulationsText.setFillColor(sf::Color::Yellow);
  sf::FloatRect textBounds = congratulationsText.getLocalBounds();
  congratulationsText.setOrigin(textBounds.left + textBounds.width / 2.0f,
                                textBounds.top + textBounds.height / 2.0f);
  congratulationsText.setPosition(window.getSize().x / 2.0f,
                                  window.getSize().y / 3.0f);

  // Play Again Button
  sf::RectangleShape restartButton;
  restartButton.setSize(sf::Vector2f(200, 60));
  restartButton.setFillColor(sf::Color(70, 130, 180)); // SteelBlue
  restartButton.setOutlineThickness(3);
  restartButton.setOutlineColor(sf::Color::White);
  restartButton.setOrigin(restartButton.getLocalBounds().width / 2.0f,
                          restartButton.getLocalBounds().height / 2.0f);
  restartButton.setPosition(window.getSize().x / 2.0f - 220,
                            window.getSize().y / 3.0f * 2.0f);

  sf::Text restartText;
  restartText.setFont(font);
  restartText.setString("Restart?");
  restartText.setCharacterSize(35);
  restartText.setFillColor(sf::Color::White);
  sf::FloatRect restartTextBounds = restartText.getLocalBounds();
  restartText.setOrigin(restartTextBounds.left + restartTextBounds.width / 2.0f,
                        restartTextBounds.top +
                            restartTextBounds.height / 2.0f);
  restartText.setPosition(restartButton.getPosition());

  // Main Menu Button
  sf::RectangleShape menuButton;
  menuButton.setSize(sf::Vector2f(200, 60));
  menuButton.setFillColor(sf::Color(60, 179, 113)); // MediumSeaGreen
  menuButton.setOutlineThickness(3);
  menuButton.setOutlineColor(sf::Color::White);
  menuButton.setOrigin(menuButton.getLocalBounds().width / 2.0f,
                       menuButton.getLocalBounds().height / 2.0f);
  menuButton.setPosition(window.getSize().x / 2.0f,
                         window.getSize().y / 3.0f * 2.0f);

  sf::Text menuText;
  menuText.setFont(font);
  menuText.setString("Title screen");
  menuText.setCharacterSize(35);
  menuText.setFillColor(sf::Color::White);
  sf::FloatRect menuTextBounds = menuText.getLocalBounds();
  menuText.setOrigin(menuTextBounds.left + menuTextBounds.width / 2.0f,
                     menuTextBounds.top + menuTextBounds.height / 2.0f);
  menuText.setPosition(menuButton.getPosition());

  // Quit Game Button
  sf::RectangleShape quitButton;
  quitButton.setSize(sf::Vector2f(200, 60));
  quitButton.setFillColor(sf::Color(178, 34, 34)); // FireBrick
  quitButton.setOutlineThickness(3);
  quitButton.setOutlineColor(sf::Color::White);
  quitButton.setOrigin(quitButton.getLocalBounds().width / 2.0f,
                       quitButton.getLocalBounds().height / 2.0f);
  quitButton.setPosition(window.getSize().x / 2.0f + 220,
                         window.getSize().y / 3.0f * 2.0f);

  sf::Text quitText;
  quitText.setFont(font);
  quitText.setString("Quit");
  quitText.setCharacterSize(35);
  quitText.setFillColor(sf::Color::White);
  sf::FloatRect quitTextBounds = quitText.getLocalBounds();
  quitText.setOrigin(quitTextBounds.left + quitTextBounds.width / 2.0f,
                     quitTextBounds.top + quitTextBounds.height / 2.0f);
  quitText.setPosition(quitButton.getPosition());

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      }

      if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Left) {
          sf::Vector2f mousePos = window.mapPixelToCoords(
              sf::Vector2i(event.mouseButton.x, event.mouseButton.y));

          if (restartButton.getGlobalBounds().contains(mousePos)) {
            return 4;
          } else if (menuButton.getGlobalBounds().contains(mousePos)) {
            return 2;
          } else if (quitButton.getGlobalBounds().contains(mousePos)) {
            return 3;
          }
        }
      }
    }

    window.clear(sf::Color::Black);

    // level stuf as background
    window.draw(fondo);
    pajaro.draw(window);
    resortera.draw(window);
    lev->render(window);

    window.draw(congratulationsText);
    window.draw(restartButton);
    window.draw(backgroundRect);
    window.draw(restartText);
    window.draw(menuButton);
    window.draw(menuText);
    window.draw(quitButton);
    window.draw(quitText);
    window.display();
  }

  return 0;
}
