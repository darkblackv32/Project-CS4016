#include "Bird.h"
#include "Constants.h" // Ensure BLOCK is defined here
#include <algorithm>   // For std::max
#include <iostream>

// static cache
std::unordered_map<BirdType, std::pair<std::unique_ptr<sf::Texture>,
                                       std::unique_ptr<sf::Texture>>>
    BirdTextureCache::cache;

Bird::Bird(BirdType birdType, sf::Vector2f pr)
    : type(birdType), pos_resortera(pr) {
  figura.setRadius(BLOCK); // BLOCK is the radius
  figura.setFillColor(sf::Color::Transparent);
  figura.setOutlineThickness(0);
  // Origin should be at the center of the circle (radius, radius)
  figura.setOrigin(BLOCK, BLOCK);
  figura.setPosition(pos_resortera);
  loadTextures();
}

void Bird::loadTextures() {
  idleTexture = nullptr;
  flyingTexture = nullptr;

  std::string idlePath, flyingPath;

  switch (type) {
  case BirdType::MILEI:
    idlePath = "assets/textures/milei/1.png";
    flyingPath = "assets/textures/milei/2.png";
    break;
  case BirdType::FUJIMORI:
    idlePath = "assets/textures/fujimori/1.png";
    flyingPath = "assets/textures/fujimori/2.png";
    break;
  case BirdType::KENJI:
    idlePath = "assets/textures/fujimori/kenji1.png";
    flyingPath = "assets/textures/fujimori/kenji2.png";
    break;
  case BirdType::MONTESINOS:
    idlePath = "assets/textures/fujimori/montesinos1.png";
    flyingPath = "assets/textures/fujimori/montesinos2.png";
    break;
  case BirdType::GARCIA:
    idlePath = "assets/textures/multi/alan1.png";
    flyingPath = "assets/textures/multi/alan2.png";
    break;
  }

  auto &entry = BirdTextureCache::cache[type];

  if (!entry.first) {
    auto idle = std::make_unique<sf::Texture>();
    if (!idle->loadFromFile(idlePath)) {
      std::cerr << "Error loading idle texture for bird type: "
                << static_cast<int>(type) << " from " << idlePath << "\n";
      return;
    }
    entry.first = std::move(idle);
  }

  if (!entry.second) {
    auto flying = std::make_unique<sf::Texture>();
    if (!flying->loadFromFile(flyingPath)) {
      std::cerr << "Error loading flying texture for bird type: "
                << static_cast<int>(type) << " from " << flyingPath << "\n";
      return;
    }
    entry.second = std::move(flying);
  }

  idleTexture = entry.first.get();
  flyingTexture = entry.second.get();

  const sf::Texture *initialTexture = idleTexture ? idleTexture : flyingTexture;
  if (initialTexture) {
    sprite.setTexture(*initialTexture, true);

    // Calculate scale to fit the sprite within the circle's diameter (2 *
    // BLOCK)
    float targetDiameter = BLOCK * 2.0f;
    float textureWidth = static_cast<float>(initialTexture->getSize().x);
    float textureHeight = static_cast<float>(initialTexture->getSize().y);

    // Scale both dimensions independently to fit the target diameter
    // This might stretch non-square textures to fit the circle, which is
    // usually desired for bird sprites
    float scaleX = targetDiameter / textureWidth;
    float scaleY = targetDiameter / textureHeight;

    sprite.setScale(scaleX, scaleY);
    // Set sprite origin to its center for correct positioning
    sprite.setOrigin(textureWidth / 2.0f, textureHeight / 2.0f);
    sprite.setPosition(figura.getPosition());
  }
}

void Bird::updateTextureState() {
  const sf::Texture *desired = lanzado ? flyingTexture : idleTexture;
  if (!desired || sprite.getTexture() == desired)
    return;

  sprite.setTexture(*desired, true);

  // Recalculate scale for the new texture
  float targetDiameter = BLOCK * 2.0f;
  float textureWidth = static_cast<float>(desired->getSize().x);
  float textureHeight = static_cast<float>(desired->getSize().y);

  float scaleX = targetDiameter / textureWidth;
  float scaleY = targetDiameter / textureHeight;

  sprite.setScale(scaleX, scaleY);
  sprite.setOrigin(textureWidth / 2.0f, textureHeight / 2.0f);
}

void Bird::reset() {
  figura.setPosition(pos_resortera);
  sprite.setPosition(pos_resortera);
  velocidad = {0, 0};
  lanzado = false;
  enResortera = true;
  updateTextureState();
}

void Bird::setBirdType(BirdType birdType) {
  if (type == birdType)
    return;
  type = birdType;
  loadTextures();       // Reload textures for new type
  updateTextureState(); // Ensure sprite state is updated after texture change
}

BirdType Bird::getBirdType() const { return type; }

void Bird::draw(sf::RenderWindow &window) { window.draw(sprite); }
