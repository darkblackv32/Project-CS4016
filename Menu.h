#ifndef MENU_H_
#define MENU_H_

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

class Menu {
private:
  void updateOptionPositions();

  sf::RenderWindow &window_;
  std::string title_;
  std::vector<std::string> options_;
  int current_page_;
  sf::Font font_;
  sf::Text title_text_;
  std::vector<sf::Text> option_texts_;

  // Buttons
  sf::Texture button_texture_left_;
  sf::Texture button_texture_right_;
  sf::Sprite button_left_;
  sf::Sprite button_right_;

  sf::Texture background_texture_;
  sf::Sprite background_sprite_;

  int level = -1;

public:
  Menu(sf::RenderWindow &window, const std::string &title,
       const std::vector<std::string> &options);
  void handleInput(const sf::Event &event);

  void draw();

  void nextPage();
  void previousPage();
  int get_level();
};

int extract_number(const std::string &s);
int render_menu(sf::RenderWindow &window);
int test_menu();

#endif // MENU_H_
