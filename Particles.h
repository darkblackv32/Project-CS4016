#ifndef PARTICLES_H_
#define PARTICLES_H_

#include <SFML/Graphics.hpp>
#include <math.h>

struct Particle {
  sf::Vector2f position;
  sf::Vector2f velocity;
  sf::Color color;
  float lifetime;        // Remaining time in seconds
  float maxLifetime;     // Total duration
  sf::CircleShape shape; // Or sf::RectangleShape, sf::Sprite
};

// Example ParticleSystem class
class ParticleSystem {
public:
  std::vector<Particle> particles;
  sf::Texture particleTexture; // Optional: if using sprites

  ParticleSystem() {
    // Load texture if needed
    // if (!particleTexture.loadFromFile("path/to/particle.png")) { ... }
  }

  void emit(sf::Vector2f position, int count) {
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

      p.color =
          sf::Color(255, rand() % 200 + 50, 0, 255); // Example: reddish-orange
      p.shape.setRadius(2.0f + (rand() % 3));        // Small random size
      p.shape.setOrigin(p.shape.getRadius(), p.shape.getRadius());
      p.shape.setFillColor(p.color);
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
      p.shape.setFillColor(p.color);
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
