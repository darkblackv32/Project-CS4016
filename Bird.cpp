#include "Bird.h"
#include "Constants.h"
#include <iostream>

// static cache
std::unordered_map<BirdType, std::pair<std::unique_ptr<sf::Texture>,
                                       std::unique_ptr<sf::Texture>>>
    BirdTextureCache::cache;

Bird::Bird(BirdType birdType, sf::Vector2f pr)
    : type(birdType), pos_resortera(pr) {
  figura.setRadius(BIRD_RADIUS);
  figura.setFillColor(sf::Color::Transparent);
  figura.setOrigin(figura.getRadius(), figura.getRadius());
  figura.setPosition(pos_resortera);
  loadTextures();
}

void Bird::loadTextures() {
  idleTexture = nullptr;
  flyingTexture = nullptr;

  BirdTextureCache::ensureDefaultTextures();

  switch (type) {
  case BirdType::MILEI:
  case BirdType::FUJIMORI: {
    auto &entry = BirdTextureCache::cache[type];
    const std::string basePath = (type == BirdType::MILEI)
                                     ? "assets/textures/milei/"
                                     : "assets/textures/fujimori/";

    if (!entry.first) {
      auto idle = std::make_unique<sf::Texture>();
      if (!idle->loadFromFile(basePath + "1.png")) {
        std::cerr << "Error loading idle texture for bird type: "
                  << static_cast<int>(type) << "\n";
        type = BirdType::DEFAULT;
        break;
      }
      entry.first = std::move(idle);
    }

    if (!entry.second) {
      auto flying = std::make_unique<sf::Texture>();
      if (!flying->loadFromFile(basePath + "2.png")) {
        std::cerr << "Error loading flying texture for bird type: "
                  << static_cast<int>(type) << "\n";
        type = BirdType::DEFAULT;
        break;
      }
      entry.second = std::move(flying);
    }

    idleTexture = entry.first.get();
    flyingTexture = entry.second.get();
    break;
  }

  case BirdType::DEFAULT:
  default:
    idleTexture = BirdTextureCache::cache[BirdType::DEFAULT].first.get();
    flyingTexture = idleTexture; // same texture for both states
    break;
  }

  const sf::Texture *initialTexture =
      idleTexture ? idleTexture : flyingTexture; // sprite
  if (initialTexture) {
    sprite.setTexture(*initialTexture, true);
    float scale = 88.0f / std::max(initialTexture->getSize().x,
                                   initialTexture->getSize().y);
    sprite.setScale(scale, scale);
    sprite.setOrigin(initialTexture->getSize().x / 2.0f,
                     initialTexture->getSize().y / 2.0f);
    sprite.setPosition(figura.getPosition());
  }
}

void Bird::updateTextureState() {
  const sf::Texture *desired = lanzado ? flyingTexture : idleTexture;
  if (!desired || sprite.getTexture() == desired)
    return;

  sprite.setTexture(*desired, true);
  float scale = 88.0f / std::max(desired->getSize().x, desired->getSize().y);
  sprite.setScale(scale, scale);
  sprite.setOrigin(desired->getSize().x / 2.0f, desired->getSize().y / 2.0f);
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
  loadTextures();
  updateTextureState();
}

BirdType Bird::getBirdType() const { return type; }

void Bird::draw(sf::RenderWindow &window) { window.draw(sprite); }
