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
  preview.text.setCharacterSize(34);
  preview.text.setFillColor(sf::Color::White);

  preview.texture = std::make_shared<sf::Texture>();
  int imageWidth = 50;
  int imageHeight = 35;
  switch (level) {
  case 0:
    preview.title = "Level 0";
    preview.text.setString("idk 0");
    if (!preview.texture->loadFromFile("assets/textures/milei/milei_portada.png")) {
      sf::Image image;
      image.create(imageWidth, imageHeight, sf::Color::Blue);
      preview.texture->loadFromImage(image);
    }
    break;
  case 1:
    preview.title = "Level 1";
    preview.text.setString("idk");
    if (!preview.texture->loadFromFile("assets/textures/milei/milei_portada.png")) {
      sf::Image image;
      image.create(imageWidth, imageHeight, sf::Color::Green);
      preview.texture->loadFromImage(image);
    }
    break;
  case 2:
    preview.title = "Level 2";
    if (!preview.texture->loadFromFile(
            "assets/textures/milei/milei_portada.png")) {
      std::cerr << "Failed to load assets/textures/milei/milei_portada.png"
                << std::endl;
      sf::Image image;
      image.create(imageWidth, imageHeight, sf::Color::Red);
      preview.texture->loadFromImage(image);
    }
    preview.text.setString("Milei vs La Casta");
    break;
  default:
    preview.title = "Unknown Level";
    preview.text.setString("Unknown");
    if (!preview.texture->loadFromFile("assets/textures/milei/milei_portada.png")) {
      sf::Image image;
      image.create(imageWidth,  imageHeight, sf::Color(128, 128, 128));
      preview.texture->loadFromImage(image);
    }
    break;
  }
  preview.sprite.setTexture(*preview.texture);
  return preview;
}
