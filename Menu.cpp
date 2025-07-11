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
  title_text_.setCharacterSize(48);  // Smaller title
  title_text_.setFillColor(sf::Color::White);
  title_text_.setPosition(
      window_size.x / 2 - title_text_.getLocalBounds().width / 2, 20);

  // Initialize arrow buttons with smaller size
  button_texture_left_.loadFromFile("assets/textures/arrow.png");
  button_texture_right_.loadFromFile("assets/textures/arrow.png");

  button_left_.setTexture(button_texture_left_);
  button_right_.setTexture(button_texture_right_);

  // Smaller button scaling
  float button_scale = 0.05f;  // Much smaller
  button_left_.setScale(button_scale, button_scale);
  button_right_.setScale(button_scale, button_scale);

  // Position left arrow (pointing left)
  button_left_.setPosition(100, window_size.y / 2);

  // Position and rotate right arrow (pointing right)
  sf::Vector2u textureSizeRight = button_texture_right_.getSize();
  button_right_.setOrigin(textureSizeRight.x / 2.0f, textureSizeRight.y / 2.0f);
  button_right_.setRotation(180);
  button_right_.setPosition(window_size.x - 100, window_size.y / 2);

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
    window_.draw(previews_[i].description);
  }

  window_.draw(button_left_);
  window_.draw(button_right_);
}

void Menu::nextPage() {
  std::cout << "Next page - Current: " << current_page_ << " Total levels: " << previews_.size() << std::endl;
  // Simple approach: just cycle through levels one by one
  if (!previews_.empty()) {
    current_page_ = (current_page_ + 1) % ((previews_.size() + OPTIONS_PER_PAGE - 1) / OPTIONS_PER_PAGE);
    updateOptionPositions();
    std::cout << "Moved to page: " << current_page_ << std::endl;
  }
}

void Menu::previousPage() {
  std::cout << "Previous page - Current: " << current_page_ << std::endl;
  if (!previews_.empty()) {
    int total_pages = (previews_.size() + OPTIONS_PER_PAGE - 1) / OPTIONS_PER_PAGE;
    current_page_ = (current_page_ - 1 + total_pages) % total_pages;
    updateOptionPositions();
    std::cout << "Moved to page: " << current_page_ << std::endl;
  }
}

void Menu::updateOptionPositions() {
  // Center a single level preview with proper sizing
  auto window_size = window_.getSize();
  int start_index = current_page_ * OPTIONS_PER_PAGE;
  int end_index = std::min(start_index + OPTIONS_PER_PAGE, (int)previews_.size());

  // Hide all previews first
  for (size_t i = 0; i < previews_.size(); ++i) {
    previews_[i].sprite.setPosition(-2000, -2000);
    previews_[i].text.setPosition(-2000, -2000);
    previews_[i].description.setPosition(-2000, -2000);
  }

  // Position the single visible preview in the center
  for (int i = start_index; i < end_index; ++i) {
    // Center horizontally and vertically
    float x_pos = window_size.x / 2;
    float y_pos = window_size.y / 2 - 20;

    // Reasonable scale for the preview image (not too big)
    float scale = 0.8f;  // Moderate scaling
    previews_[i].sprite.setScale(scale, scale);

    // Center the sprite by adjusting for its size
    sf::FloatRect bounds = previews_[i].sprite.getLocalBounds();
    previews_[i].sprite.setPosition(x_pos - (bounds.width * scale) / 2,
                                   y_pos - (bounds.height * scale) / 2);

    // Position text below the sprite, centered
    sf::FloatRect textBounds = previews_[i].text.getLocalBounds();
    previews_[i].text.setPosition(x_pos - textBounds.width / 2,
                                 y_pos + (bounds.height * scale) / 2 + 20);

    sf::FloatRect descBounds = previews_[i].description.getLocalBounds();
    previews_[i].description.setOrigin(descBounds.left + descBounds.width / 2.0f,
                                      descBounds.top + descBounds.height / 2.0f);
    previews_[i].description.setPosition(
        x_pos, y_pos + (bounds.height * scale) / 2 + 60 + descBounds.height / 2.0f);

    std::cout << "Centered level " << i << " at (" << x_pos << ", " << y_pos << ")" << std::endl;
  }
}

int Menu::get_level() { return level; }

int render_menu(sf::RenderWindow &window) {
  // Only 3 levels available
  std::vector<LevelPreview> previews;
  for (int i = 0; i < 4; ++i) {
    previews.push_back(get_level_preview(i));
  }

  Menu menu(window, "Level Select", previews);
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
