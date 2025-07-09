#include "Levels.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>

LevelPreview get_level_preview(int level) {
  LevelPreview preview;
  preview.font = std::make_shared<sf::Font>();
  if (!preview.font->loadFromFile("assets/fonts/angrybirds-regular.ttf")) {
    std::cerr << "Failed to load font assets/fonts/angrybirds-regular.ttf"
              << std::endl;
  }
  preview.text.setFont(*preview.font);
  preview.text.setCharacterSize(32);  // Reasonable text size
  preview.text.setFillColor(sf::Color::White);

  preview.texture = std::make_shared<sf::Texture>();
  int imageWidth = 320;
  int imageHeight = 240;

  switch (level) {
  case 0:
    preview.title = "Level 1";
    preview.text.setString("Tutorial");
    if (!preview.texture->loadFromFile("assets/textures/milei/milei_portada.png")) {
      sf::Image image;
      image.create(imageWidth, imageHeight, sf::Color::Blue);
      preview.texture->loadFromImage(image);
    }
    break;
  case 1:
    preview.title = "Level 2";
    preview.text.setString("Easy");
    if (!preview.texture->loadFromFile("assets/textures/milei/milei_portada.png")) {
      sf::Image image;
      image.create(imageWidth, imageHeight, sf::Color::Green);
      preview.texture->loadFromImage(image);
    }
    break;
  case 2:
    preview.title = "Level 3";
    preview.text.setString("Milei vs La Casta");
    if (!preview.texture->loadFromFile("assets/textures/milei/milei_portada.png")) {
      sf::Image image;
      image.create(imageWidth, imageHeight, sf::Color::Red);
      preview.texture->loadFromImage(image);
    }
    break;
  case 3:
    preview.title = "Level 4";
    preview.text.setString("Medium");
    if (!preview.texture->loadFromFile("assets/textures/fujimori/1.png")) {
      sf::Image image;
      image.create(imageWidth, imageHeight, sf::Color::Yellow);
      preview.texture->loadFromImage(image);
    }
    break;
  case 4:
    preview.title = "Level 5";
    preview.text.setString("Hard");
    if (!preview.texture->loadFromFile("assets/textures/fujimori/2.png")) {
      sf::Image image;
      image.create(imageWidth, imageHeight, sf::Color::Magenta);
      preview.texture->loadFromImage(image);
    }
    break;
  case 5:
    preview.title = "Level 6";
    preview.text.setString("Expert");
    if (!preview.texture->loadFromFile("assets/textures/milei/milei_portada.png")) {
      sf::Image image;
      image.create(imageWidth, imageHeight, sf::Color::Cyan);
      preview.texture->loadFromImage(image);
    }
    break;
  default:
    preview.title = "Level " + std::to_string(level + 1);
    preview.text.setString("Unknown");
    if (!preview.texture->loadFromFile("assets/textures/milei/milei_portada.png")) {
      sf::Image image;
      image.create(imageWidth, imageHeight, sf::Color(128, 128, 128));
      preview.texture->loadFromImage(image);
    }
    break;
  }
  preview.sprite.setTexture(*preview.texture);
  return preview;
}
