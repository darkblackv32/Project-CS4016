#pragma once
#include "Constants.h"
#include <SFML/Graphics.hpp>
#include <memory>
#include <string>
#include <unordered_map>

enum class BirdType { DEFAULT, MILEI, FUJIMORI };

struct BirdTextureCache; // forward declaration for texture cache

struct Bird {
  sf::CircleShape figura; // collision and physics
  sf::Sprite sprite;      // rendering
  sf::Vector2f velocidad;
  bool lanzado = false;
  bool enResortera = true;
  BirdType type;
  sf::Vector2f pos_resortera;

  // textures (managed via cache)
  sf::Texture *idleTexture = nullptr;
  sf::Texture *flyingTexture = nullptr;

  Bird(BirdType birdType = BirdType::DEFAULT,
       sf::Vector2f pr = sf::Vector2f(100.0f, 470.0f));
  void updatePhysics(float deltaTime);
  void reset();
  void loadTextures();
  void updateTextureState();
  void setBirdType(BirdType birdType);
  BirdType getBirdType() const;
  void draw(sf::RenderWindow &window);
};
struct BirdTextureCache { // texture cache implementation
  static std::unordered_map<BirdType, std::pair<std::unique_ptr<sf::Texture>,
                                                std::unique_ptr<sf::Texture>>>
      cache;

  static void ensureDefaultTextures() {
    if (cache.find(BirdType::DEFAULT) == cache.end()) {
      auto texture = std::make_unique<sf::Texture>();
      createDefaultTexture(*texture);
      // both states use same texture for default bird
      cache[BirdType::DEFAULT] = {std::move(texture), nullptr};
    }
  }

  static void createDefaultTexture(sf::Texture &texture) {
    texture.create(64, 64);
    std::vector<sf::Uint8> pixels(64 * 64 * 4, 0);

    const sf::Color baseColor(215, 38, 38);
    const sf::Color eyeWhite(255, 255, 255);
    const sf::Color eyeBlack(0, 0, 0);
    const sf::Color beak(254, 184, 19);
    const sf::Color eyebrow(0, 0, 0);

    for (int y = 0; y < 64; ++y) {
      for (int x = 0; x < 64; ++x) {
        int idx = (y * 64 + x) * 4;
        float dx = x - 32.0f;
        float dy = y - 32.0f;
        float distSq = dx * dx + dy * dy;

        if (distSq > 32.0f * 32.0f)
          continue;

        // Base body
        pixels[idx] = baseColor.r;
        pixels[idx + 1] = baseColor.g;
        pixels[idx + 2] = baseColor.b;
        pixels[idx + 3] = 255;

        // Highlight
        if (distSq < 0.85f * 32.0f * 32.0f) {
          pixels[idx] = std::min(255, baseColor.r + 20);
          pixels[idx + 1] = std::min(255, baseColor.g + 20);
          pixels[idx + 2] = std::min(255, baseColor.b + 20);
        }

        // Eyes
        if (dx < -8 && dx > -20 && dy < 0 && dy > -12) {
          std::copy_n(&eyeWhite.r, 4, &pixels[idx]);
        } else if (dx > 8 && dx < 20 && dy < 0 && dy > -12) {
          std::copy_n(&eyeWhite.r, 4, &pixels[idx]);
        }

        // Pupils
        if (dx < -11 && dx > -17 && dy < -2 && dy > -10) {
          std::copy_n(&eyeBlack.r, 4, &pixels[idx]);
        } else if (dx > 11 && dx < 17 && dy < -2 && dy > -10) {
          std::copy_n(&eyeBlack.r, 4, &pixels[idx]);
        }

        // Beak
        if (dx > -10 && dx < 10 && dy > 0 && dy < 16 &&
            dy > -0.8f * std::abs(dx)) {
          std::copy_n(&beak.r, 4, &pixels[idx]);
        }

        // Eyebrows
        if (dx < -8 && dx > -22 && dy > -18 && dy < -14 &&
            dy < -0.5f * dx - 20) {
          std::copy_n(&eyebrow.r, 4, &pixels[idx]);
        } else if (dx > 8 && dx < 22 && dy > -18 && dy < -14 &&
                   dy < 0.5f * dx - 20) {
          std::copy_n(&eyebrow.r, 4, &pixels[idx]);
        }
      }
    }
    texture.update(pixels.data());
  }
};
