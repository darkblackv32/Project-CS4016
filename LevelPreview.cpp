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
  preview.text.setCharacterSize(42);  // Reasonable text size
  preview.text.setFillColor(sf::Color::White);

  preview.description.setFont(*preview.font);
  preview.description.setCharacterSize(34);
  preview.description.setFillColor(sf::Color::White);
  preview.description.setLineSpacing(1.2f);

  preview.texture = std::make_shared<sf::Texture>();
  int imageWidth = 640;
  int imageHeight = 480;

  sf::Image originalImage;
  bool imageLoaded = false;

  switch (level) {
  case 0:
    preview.title = "Level 1";
    preview.text.setString("Tutorial");
    preview.description.setString("A simple tutorial to\nget you started.");
    imageLoaded = originalImage.loadFromFile("assets/textures/multi/multi_portada.png");
    if (!imageLoaded) {
      originalImage.create(imageWidth, imageHeight, sf::Color::Blue);
    }
    break;
  case 1:
    preview.title = "Level 2";
    preview.text.setString("Fujimori y los vladivideos");
    preview.description.setString("Bienvenido a las instalaciones secretas del SIN \n"
                                  "(Servicio de Inteligencia Nacional), ayuda a los Fujimori \n"
                                  " a recuperar los vladivideos mientras esquivan \n"
                                  "a la prensa y la justicia");
    imageLoaded = originalImage.loadFromFile("assets/textures/fujimori/fujimori_portada.png");
    if (!imageLoaded) {
      originalImage.create(imageWidth, imageHeight, sf::Color::Green);
    }
    break;
  case 2:
    preview.title = "Level 3";
    preview.text.setString("Milei vs La Casta");
    preview.description.setString("La Casta politica ha secuestrado a Conan,\n"
                                  " el perro clonado de Milei. Ayuda a Milei a recuperar \n"
                                  " a su perro mientras esquivas a los miembros de la Casta \n"
                                  "y destruyes ministerios");
    imageLoaded = originalImage.loadFromFile("assets/textures/milei/milei_portada.png");
    if (!imageLoaded) {
      originalImage.create(imageWidth, imageHeight, sf::Color::Red);
    }
    break;
  case 3:
    preview.title = "Level 4";
    preview.text.setString("Medium");
    preview.description.setString("A medium difficulty level.");
    imageLoaded = originalImage.loadFromFile("assets/textures/fujimori/1.png");
    if (!imageLoaded) {
      originalImage.create(imageWidth, imageHeight, sf::Color::Yellow);
    }
    break;
  case 4:
    preview.title = "Level 5";
    preview.text.setString("Hard");
    preview.description.setString("A hard difficulty level.");
    imageLoaded = originalImage.loadFromFile("assets/textures/fujimori/2.png");
    if (!imageLoaded) {
      originalImage.create(imageWidth, imageHeight, sf::Color::Magenta);
    }
    break;
  case 5:
    preview.title = "Level 6";
    preview.text.setString("Expert");
    preview.description.setString("An expert difficulty level.");
    imageLoaded = originalImage.loadFromFile("assets/textures/milei/milei_portada.png");
    if (!imageLoaded) {
      originalImage.create(imageWidth, imageHeight, sf::Color::Cyan);
    }
    break;
  default:
    preview.title = "Level " + std::to_string(level + 1);
    preview.text.setString("Unknown");
    preview.description.setString("No description available.");
    imageLoaded = originalImage.loadFromFile("assets/textures/milei/milei_portada.png");
    if (!imageLoaded) {
      originalImage.create(imageWidth, imageHeight, sf::Color(128, 128, 128));
    }
    break;
  }

  sf::Texture originalTexture;
  originalTexture.loadFromImage(originalImage);

  sf::Sprite tempSprite(originalTexture);
  tempSprite.setScale((float)imageWidth / originalTexture.getSize().x,
                    (float)imageHeight / originalTexture.getSize().y);

  sf::RenderTexture renderTexture;
  renderTexture.create(imageWidth, imageHeight);
  renderTexture.clear();
  renderTexture.draw(tempSprite);
  renderTexture.display();

  *preview.texture = renderTexture.getTexture();

  preview.sprite.setTexture(*preview.texture);
  return preview;
}
