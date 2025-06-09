#include "Bird.h"
#include "Constants.h"
#include "Physics.h"

Bird::Bird() {
  figura.setRadius(22.0f);
  figura.setFillColor(sf::Color(255, 60, 60));
  figura.setOrigin(22.0f, 22.0f);
  figura.setPosition(POS_RESORTERA);
}

void Bird::updatePhysics(float deltaTime) {
  Physics::applyAirResistance(velocidad, deltaTime);
  velocidad.y += GRAVEDAD * deltaTime;
  figura.move(velocidad * deltaTime);
}

void Bird::reset() {
  figura.setPosition(POS_RESORTERA);
  velocidad = {0, 0};
  lanzado = false;
  enResortera = true;
}
