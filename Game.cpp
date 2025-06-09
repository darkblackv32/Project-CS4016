#include "Game.h"
#include "Bird.h"
#include "Constants.h"
#include "Levels.h"
#include "Physics.h"
#include "Slingshot.h"
#include <vector>

void render_bird_game(sf::RenderWindow &ventana, int level, int width,
                      int height) {
  Bird pajaro;
  Slingshot resortera;
  bool arrastrando = false;
  sf::Vector2f clickOffset;
  std::vector<sf::Vector2f> trayectoria;

  sf::RectangleShape fondo({800.0f, 600.0f});
  fondo.setFillColor({135, 206, 235});

  Level *lev = return_level(level, width, height);

  while (ventana.isOpen()) {
    sf::Event evento;
    while (ventana.pollEvent(evento)) {
      if (evento.type == sf::Event::Closed)
        ventana.close();

      if (evento.type == sf::Event::MouseButtonPressed) {
        sf::Vector2f mousePos =
            ventana.mapPixelToCoords(sf::Mouse::getPosition(ventana));
        if (pajaro.figura.getGlobalBounds().contains(mousePos) &&
            !pajaro.lanzado) {
          arrastrando = true;
          clickOffset = pajaro.figura.getPosition() - mousePos;
        }
      }

      if (evento.type == sf::Event::MouseButtonReleased && arrastrando) {
        arrastrando = false;
        pajaro.lanzado = true;

        sf::Vector2f direccion = POS_RESORTERA - pajaro.figura.getPosition();
        pajaro.velocidad = direccion * FUERZA_MULTIPLICADOR;
      }
    }

    if (arrastrando) {
      sf::Vector2f mousePos =
          ventana.mapPixelToCoords(sf::Mouse::getPosition(ventana));
      sf::Vector2f nuevaPos = mousePos + clickOffset;

      sf::Vector2f vectorResortera = nuevaPos - POS_RESORTERA;
      float distancia = std::hypot(vectorResortera.x, vectorResortera.y);

      if (distancia > RADIO_MAX_RESORTERA) {
        vectorResortera = (vectorResortera / distancia) * RADIO_MAX_RESORTERA;
        nuevaPos = POS_RESORTERA + vectorResortera;
      }

      pajaro.figura.setPosition(nuevaPos);

      trayectoria = Physics::calcularTrayectoria(
          nuevaPos, (POS_RESORTERA - nuevaPos) * FUERZA_MULTIPLICADOR,
          0.0015f, // Densidad del aire
          0.08f    // Área transversal
      );
    }

    if (pajaro.lanzado) {
      pajaro.updatePhysics(0.016f);

      if (pajaro.figura.getPosition().y + pajaro.figura.getRadius() > 550.0f) {
        Physics::handleGroundCollision(pajaro.figura, pajaro.velocidad, 550.0f);
      }

      if (std::abs(pajaro.velocidad.x) < 15.0f &&
          std::abs(pajaro.velocidad.y) < 15.0f &&
          pajaro.figura.getPosition().y + pajaro.figura.getRadius() >=
              550.0f - pajaro.figura.getRadius()) {
        pajaro.reset();
      }
    }

    ventana.clear();

    ventana.draw(fondo);
    resortera.draw(ventana);
    lev->render(ventana);

    if (arrastrando || !pajaro.lanzado) {
      resortera.updateBands(pajaro.figura.getPosition(), arrastrando);
    }

    if (arrastrando) {
      for (size_t i = 0; i < trayectoria.size(); ++i) {
        float alpha =
            120.0f * (1.0f - static_cast<float>(i) / trayectoria.size());
        float size = 2.5f * (1.0f + i * 0.05f);
        sf::CircleShape puntoTrazo(size);
        puntoTrazo.setFillColor(
            sf::Color(0, 0, 0, static_cast<sf::Uint8>(alpha)));
        puntoTrazo.setOrigin(size, size);
        puntoTrazo.setPosition(trayectoria[i]);
        ventana.draw(puntoTrazo);
      }
    }

    ventana.draw(pajaro.figura);
    ventana.display();
  }
}
