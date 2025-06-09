#include "Bird.h"
#include "Constants.h"
#include "Physics.h"
#include <iostream>

Bird::Bird(BirdType birdType) : type(birdType) {
  figura.setRadius(22.0f);
  figura.setFillColor(sf::Color::White);
  figura.setOrigin(22.0f, 22.0f);
  figura.setPosition(POS_RESORTERA);
  loadTexture();
}

void Bird::loadTexture() {
  useSprite = false; // Default to using the circle shape

  switch (type) {
    case BirdType::MILEI:
      if (!texture.loadFromFile("milei.png")) {
        std::cerr << "Error loading milei.png texture!" << std::endl;
        createDefaultTexture();
      } else {
        useSprite = true;
        sprite.setTexture(texture);
        float scale = 44.0f / std::max(texture.getSize().x, texture.getSize().y);
        sprite.setScale(scale, scale);
        sprite.setOrigin(texture.getSize().x / 2.0f, texture.getSize().y / 2.0f);
        sprite.setPosition(figura.getPosition());
      }
      break;
    case BirdType::FUJIMORI:
      if (!texture.loadFromFile("fujimori.png")) {
        std::cerr << "Error loading fujimori.png texture!" << std::endl;
        createDefaultTexture();
      } else {
        useSprite = true;
        sprite.setTexture(texture);
        float scale = 44.0f / std::max(texture.getSize().x, texture.getSize().y);
        sprite.setScale(scale, scale);
        sprite.setOrigin(texture.getSize().x / 2.0f, texture.getSize().y / 2.0f);
        sprite.setPosition(figura.getPosition());
      }
      break;
    case BirdType::DEFAULT:
    default:
      createDefaultTexture();
      figura.setTexture(&texture);
      break;
  }
}

void Bird::createDefaultTexture() {
  texture.create(64, 64);
  sf::Uint8* pixels = new sf::Uint8[64 * 64 * 4];
  
  sf::Color baseColor(215, 38, 38);
  sf::Color eyeWhiteColor(255, 255, 255);
  sf::Color eyeBlackColor(0, 0, 0);
  sf::Color beakColor(254, 184, 19);
  sf::Color eyebrowColor(0, 0, 0);
  
  for (int y = 0; y < 64; ++y) {
    for (int x = 0; x < 64; ++x) {
      int idx = (y * 64 + x) * 4;
      
      float dx = x - 32.0f;
      float dy = y - 32.0f;
      float distSq = dx*dx + dy*dy;
      
      if (distSq > 32.0f * 32.0f) {
        pixels[idx] = 0;
        pixels[idx+1] = 0;
        pixels[idx+2] = 0;
        pixels[idx+3] = 0;
        continue;
      }
      
      pixels[idx] = baseColor.r;
      pixels[idx+1] = baseColor.g;
      pixels[idx+2] = baseColor.b;
      pixels[idx+3] = 255;
      
      float factor = 1.0f - distSq / (32.0f * 32.0f);
      if (factor > 0.85f) {
        pixels[idx] = std::min(255, int(pixels[idx] + 20));
        pixels[idx+1] = std::min(255, int(pixels[idx+1] + 20));
        pixels[idx+2] = std::min(255, int(pixels[idx+2] + 20));
      }
      
      if (dx > -20 && dx < -8 && dy > -12 && dy < 0) {
        pixels[idx] = eyeWhiteColor.r;
        pixels[idx+1] = eyeWhiteColor.g;
        pixels[idx+2] = eyeWhiteColor.b;
      }
      
      if (dx > 8 && dx < 20 && dy > -12 && dy < 0) {
        pixels[idx] = eyeWhiteColor.r;
        pixels[idx+1] = eyeWhiteColor.g;
        pixels[idx+2] = eyeWhiteColor.b;
      }
      
      if (dx > -17 && dx < -11 && dy > -10 && dy < -2) {
        pixels[idx] = eyeBlackColor.r;
        pixels[idx+1] = eyeBlackColor.g;
        pixels[idx+2] = eyeBlackColor.b;
      }
      
      if (dx > 11 && dx < 17 && dy > -10 && dy < -2) {
        pixels[idx] = eyeBlackColor.r;
        pixels[idx+1] = eyeBlackColor.g;
        pixels[idx+2] = eyeBlackColor.b;
      }
      
      if (dx > -10 && dx < 10 && dy > 0 && dy < 16 && dy > -0.8f * std::abs(dx)) {
        pixels[idx] = beakColor.r;
        pixels[idx+1] = beakColor.g;
        pixels[idx+2] = beakColor.b;
      }
      
      if (dx > -22 && dx < -8 && dy > -18 && dy < -14 && dy < -0.5f * dx - 20) {
        pixels[idx] = eyebrowColor.r;
        pixels[idx+1] = eyebrowColor.g;
        pixels[idx+2] = eyebrowColor.b;
      }
      
      if (dx > 8 && dx < 22 && dy > -18 && dy < -14 && dy < 0.5f * dx - 20) {
        pixels[idx] = eyebrowColor.r;
        pixels[idx+1] = eyebrowColor.g;
        pixels[idx+2] = eyebrowColor.b;
      }
    }
  }
  
  texture.update(pixels);
  delete[] pixels;
  
  // Use the circle shape with texture for the default bird
  useSprite = false;
  figura.setTexture(&texture);
}

void Bird::updatePhysics(float deltaTime) {
  Physics::applyAirResistance(velocidad, deltaTime);
  velocidad.y += GRAVEDAD * deltaTime;
  figura.move(velocidad * deltaTime);
  
  if (useSprite) {
    sprite.setPosition(figura.getPosition());
  }
}

void Bird::reset() {
  figura.setPosition(POS_RESORTERA);
  if (useSprite) {
    sprite.setPosition(POS_RESORTERA);
  }
  velocidad = {0, 0};
  lanzado = false;
  enResortera = true;
}

void Bird::setBirdType(BirdType birdType) {
  if (type != birdType) {
    type = birdType;
    loadTexture();
  }
}

BirdType Bird::getBirdType() const {
  return type;
}

void Bird::draw(sf::RenderWindow& window) {
  if (useSprite) {
    window.draw(sprite);
  } else {
    window.draw(figura);
  }
}