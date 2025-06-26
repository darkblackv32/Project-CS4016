#include "Menu.h"
#include "Constants.h"

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <vector>

int extract_number(const std::string &s) {
  size_t last_space = s.rfind(' ');
  if (last_space != std::string::npos) {
    return std::stoi(s.substr(last_space + 1));
  }
  // If no space is found, return -1
  // Lest make an agreement and promise that all the levels end with a number
  return std::stoi(s);
}

Menu::Menu(sf::RenderWindow &window, const std::string &title,
           const std::vector<std::string> &options)
    : window_(window), title_(title), options_(options), current_page_(0) {
  if (!background_texture_.loadFromFile("assets/textures/portada_clean.jpg")) {
    std::cout << "Failed to load background image." << std::endl;
  }
  background_sprite_.setTexture(background_texture_);
  background_sprite_.setScale(
      (float)window.getSize().x / background_texture_.getSize().x,
      (float)window.getSize().y / background_texture_.getSize().y);

  if (!font_.loadFromFile("./assets/fonts/angrybirds-regular.ttf")) {
    std::cout << "Couldn't open font" << std::endl;
    exit(0);
  }

  auto window_size = window.getSize();

  title_text_.setFont(font_);
  title_text_.setString(title_);
  title_text_.setCharacterSize(72);
  title_text_.setFillColor(sf::Color::White);
  title_text_.setPosition(
      window_size.x / 2 - title_text_.getLocalBounds().width / 2, 20);

  // Initialize option texts
  for (const auto &option : options_) {
    sf::Text text;
    text.setFont(font_);
    text.setString(option);
    text.setCharacterSize(30);
    text.setFillColor(sf::Color::White);
    option_texts_.push_back(text);
  }

  // Initialize button
  button_texture_left_.loadFromFile("assets/textures/arrow.png");
  button_texture_right_.loadFromFile("assets/textures/arrow.png");

  button_left_.setTexture(button_texture_left_);
  button_right_.setTexture(button_texture_right_);

  button_left_.setPosition(SPACE_BETWEEN_ENDS_BUTTONS, window_size.y - 50);
  button_right_.setPosition(window_size.x - SPACE_BETWEEN_ENDS_BUTTONS,
                            window_size.y - 50);

  // Scale the buttons
  sf::Vector2u textureSizeLeft = button_texture_left_.getSize();
  button_left_.setScale(BUTTON_SCALE_X / textureSizeLeft.x,
                        BUTTON_SCALE_Y / textureSizeLeft.y);

  sf::Vector2u textureSizeRight = button_texture_right_.getSize();
  button_right_.setScale(BUTTON_SCALE_X / textureSizeRight.x,
                         BUTTON_SCALE_Y / textureSizeRight.y);

  button_right_.setRotation(180);

  updateOptionPositions();
}

void Menu::handleInput(const sf::Event &event) {
  if (event.type == sf::Event::MouseButtonPressed) {
    if (event.mouseButton.button == sf::Mouse::Left) {
      sf::Vector2f mouse_pos =
          window_.mapPixelToCoords({event.mouseButton.x, event.mouseButton.y});

      if (button_left_.getGlobalBounds().contains(mouse_pos)) {
        previousPage();
      } else if (button_right_.getGlobalBounds().contains(mouse_pos)) {
        nextPage();
      } else {
        // Check for option selection
        int start_index = current_page_ * OPTIONS_PER_PAGE;
        int end_index =
            std::min(start_index + OPTIONS_PER_PAGE, (int)options_.size());

        for (int i = start_index; i < end_index; ++i) {
          if (option_texts_[i].getGlobalBounds().contains(mouse_pos)) {
            level = extract_number(options_[i]);
            break;
          }
        }
      }
    }
  }
}

void Menu::draw() {
  window_.draw(background_sprite_);
  window_.draw(title_text_);

  // Calculate the range of options to display on the current page
  int start_index = current_page_ * OPTIONS_PER_PAGE;
  int end_index =
      std::min(start_index + OPTIONS_PER_PAGE, (int)options_.size());

  // Draw the options for the current page
  for (int i = start_index; i < end_index; ++i) {
    window_.draw(option_texts_[i]);
  }

  window_.draw(button_left_);
  window_.draw(button_right_);
}

void Menu::nextPage() {
  if ((current_page_ + 1) * OPTIONS_PER_PAGE < options_.size()) {
    current_page_++;
    updateOptionPositions();
  }
}

void Menu::previousPage() {
  if (current_page_ > 0) {
    current_page_--;
    updateOptionPositions();
  }
}

void Menu::updateOptionPositions() {
  auto window_size = window_.getSize();

  float start_x = SPACE_BETWEEN_ENDS_BUTTONS + 10;
  float start_y = this->title_text_.getLocalBounds().getPosition().y +
                  this->title_text_.getLocalBounds().getSize().y + 100;
  option_texts_[0].getLocalBounds().width;
  float x_offset = (window_size.x +
                    option_texts_[0].getLocalBounds().width * OPTIONS_PER_ROW) /
                   OPTIONS_PER_ROW;
  float y_offset = (window_size.y +
                    option_texts_[0].getLocalBounds().height *
                        (OPTIONS_PER_PAGE / OPTIONS_PER_ROW) -
                    start_y) /
                   (OPTIONS_PER_PAGE / OPTIONS_PER_ROW);

  // Calculate the range of options to display on the current page
  int start_index = current_page_ * OPTIONS_PER_PAGE;
  int end_index =
      std::min(start_index + OPTIONS_PER_PAGE, (int)options_.size());

  // Update the position of each option text on the current page
  for (int i = start_index; i < end_index; ++i) {
    int index_on_page = i - start_index;
    int row = index_on_page / OPTIONS_PER_ROW;
    int col = index_on_page % OPTIONS_PER_ROW;

    option_texts_[i].setPosition(start_x + col * x_offset,
                                 start_y + row * y_offset);
  }
}

int Menu::get_level() { return this->level; }

int render_menu(sf::RenderWindow &window) {
  // Make this dynamic if possible
  std::vector<std::string> levels = {
      "Level 0",
      "Level 1",
      "Level 2",
  };
  Menu menu(window, "Main Menu", levels);
  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
        return -1;
      }
      menu.handleInput(event);
    }

    if (menu.get_level() > -1) {
      return menu.get_level();
    }

    // dark blue background
    window.clear(sf::Color(50, 50, 150));
    menu.draw();
    window.display();
  }

  // Shouldn't happen
  return -1;
}

int test_menu() {
  sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Menu");
  window.setFramerateLimit(60);

  std::vector<std::string> options = {
      "Level 0",
      "Level 1",
      "Level 2",
  };

  Menu menu(window, "Main Menu", options);

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      }
      menu.handleInput(event);
    }

    // dark blue background
    window.clear(sf::Color(50, 50, 150));
    menu.draw();
    window.display();
  }

  return 0;
}
