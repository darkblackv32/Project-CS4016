#ifndef PARTICLES_H_
#define PARTICLES_H_

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <math.h>

struct Particle {
  sf::Vector2f position;
  sf::Vector2f velocity;
  sf::Color color;
  float lifetime;    // Remaining time in seconds
  float maxLifetime; // Total duration
  sf::Sprite shape;  // Or sf::RectangleShape, sf::Sprite
};

// Example ParticleSystem class
class ParticleSystem {
public:
  std::vector<Particle> particles;

  ParticleSystem() {}

  void emit(sf::Vector2f position, int count, sf::Texture &particleTexture) {
    for (int i = 0; i < count; ++i) {
      Particle p;
      p.position = position;
      // Random velocity for explosion effect
      float angle = (rand() % 360) * 3.14159f / 180.0f;
      float speed = (rand() % 100) + 50.0f; // Random speed between 50 and 150
      p.velocity =
          sf::Vector2f(std::cos(angle) * speed, std::sin(angle) * speed);

      p.maxLifetime =
          (rand() % 100 + 50) / 100.0f; // Lifetime between 0.5 and 1.5 seconds
      p.lifetime = p.maxLifetime;

      p.color = sf::Color(255, 255, 255, 255);
      p.shape.setTexture(particleTexture);
      p.shape.setScale(0.1, 0.1);
      p.shape.setOrigin(p.shape.getTextureRect().width / 2.0f,
                        p.shape.getTextureRect().height / 2.0f);
      p.shape.setColor(p.color);
      p.shape.setPosition(p.position);

      particles.push_back(p);
    }
  }

  void update(float dt) {
    for (size_t i = 0; i < particles.size();) {
      Particle &p = particles[i];
      p.position += p.velocity * dt;
      p.lifetime -= dt;

      // Apply gravity or damping if desired
      // p.velocity.y += 9.8f * dt; // Simple gravity

      // Fade out particle
      sf::Uint8 alpha =
          static_cast<sf::Uint8>(255 * (p.lifetime / p.maxLifetime));
      p.color.a = alpha;
      p.shape.setColor(p.color);
      p.shape.setPosition(p.position);

      if (p.lifetime <= 0) {
        particles.erase(particles.begin() + i); // Remove dead particle
      } else {
        ++i; // Move to next particle
      }
    }
  }

  void draw(sf::RenderWindow &window) {
    for (const auto &p : particles) {
      window.draw(p.shape); // Draw the particle's shape
    }
  }
};

#endif // PARTICLES_H_
