#include "Physics.h"

std::vector<sf::Vector2f> Physics::calcularTrayectoria(sf::Vector2f inicio,
                                                       sf::Vector2f velocidad,
                                                       float airDensity,
                                                       float crossArea) {
  std::vector<sf::Vector2f> puntos;
  const int pasos = 150;
  float dt = 0.03f;

  sf::Vector2f currentVel = velocidad;
  sf::Vector2f currentPos = inicio;

  for (int i = 0; i < pasos; ++i) {
    // resistencia del aire (F = 0.5 * ρ * v² * C * A)
    float speed = std::hypot(currentVel.x, currentVel.y);
    sf::Vector2f drag = -0.5f * airDensity * speed * crossArea * currentVel;

    currentVel += (sf::Vector2f(0.0f, GRAVEDAD) + drag) * dt;
    currentPos += currentVel * dt;

    // Make not constant. Use the bottom of the level
    // TODO
    float bottom = 650;
    if (currentPos.y > bottom) {
      // simular rebote
      currentVel.y *= -COEF_RESTITUCION;
      currentVel.x *= FRICCION_SUELO;
      currentPos.y = bottom - 22.0f;
    }

    puntos.push_back(currentPos);

    if (speed < 50.0f && currentPos.y >= bottom - 22.0f)
      break;
  }
  return puntos;
}

void Physics::applyAirResistance(sf::Vector2f &velocidad, float deltaTime,
                                 float airDensity, float crossArea) {
  float speed = std::hypot(velocidad.x, velocidad.y);
  if (speed > 0) {
    sf::Vector2f drag = -0.5f * airDensity * speed * crossArea * velocidad;
    velocidad += drag * deltaTime;
  }
}

void Physics::handleGroundCollision(sf::CircleShape &figura,
                                    sf::Vector2f &velocidad,
                                    float groundLevel) {
  if (figura.getPosition().y + figura.getRadius() > groundLevel) {
    figura.setPosition(figura.getPosition().x,
                       groundLevel - figura.getRadius());
    velocidad.y *= -COEF_RESTITUCION;
    velocidad.x *= FRICCION_SUELO;
  }
}
