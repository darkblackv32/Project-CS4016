#include "Menu.h"
#include "Constants.h"
#include "Levels.h"

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
           std::vector<LevelPreview> &previews)
    : window_(window), title_(title), previews_(previews), current_page_(0) {
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
            std::min(start_index + OPTIONS_PER_PAGE, (int)previews_.size());

        for (int i = start_index; i < end_index; ++i) {
          if (previews_[i].sprite.getGlobalBounds().contains(mouse_pos)) {
            level = i;
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
      std::min(start_index + OPTIONS_PER_PAGE, (int)previews_.size());

  // Draw the options for the current page
  for (int i = start_index; i < end_index; ++i) {
    window_.draw(previews_[i].sprite);
    window_.draw(previews_[i].text);
  }

  window_.draw(button_left_);
  window_.draw(button_right_);
}

void Menu::nextPage() {
  if ((current_page_ + 1) * OPTIONS_PER_PAGE < previews_.size()) {
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
  int start_index = current_page_ * OPTIONS_PER_PAGE;
  int end_index =
      std::min(start_index + OPTIONS_PER_PAGE, (int)previews_.size());

  for (int i = start_index; i < end_index; ++i) {
    int option_index_on_page = i - start_index;
    previews_[i].sprite.setPosition(
        (option_index_on_page + 1) * SPRITE_WIDTH_WITH_PADDING,
        window_.getSize().y / 2);
    previews_[i].text.setPosition(
        (option_index_on_page + 1) * SPRITE_WIDTH_WITH_PADDING,
        window_.getSize().y / 2 + 100);
  }
}

int Menu::get_level() { return level; }

int render_menu(sf::RenderWindow &window) {
  // Make this dynamic if possible
  std::vector<LevelPreview> previews;
  for (int i = 0; i < 3; ++i) {
    previews.push_back(get_level_preview(i));
  }

  Menu menu(window, "Main Menu", previews);
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
