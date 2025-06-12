#include "Game.h"

#include <iostream>

#include "Bird.h"
#include "Constants.h"
#include "Levels.h"
#include "Physics.h"
#include "Slingshot.h"
#include <vector>

#include <SFML/Audio.hpp>

const float TRAJECTORY_STEP = 0.0015f;
const float GRAVITY = 0.08f;

void playBirdSound(BirdType birdType) {
  static sf::SoundBuffer mileiBuffer;
  static sf::SoundBuffer fujimoriBuffer;
  static bool buffersLoaded = false;
  static sf::Sound sound;

  if (!buffersLoaded) {
    bool loaded = true;
    if (!mileiBuffer.loadFromFile("audios/milei.mp3")) {
      std::cerr << "Failed to load audios/milei.mp3" << std::endl;
      loaded = false;
    }
    if (!fujimoriBuffer.loadFromFile("audios/fujimori.mp3")) {
      std::cerr << "Failed to load audios/fujimori.mp3" << std::endl;
      loaded = false;
    }
    buffersLoaded = loaded;
    if (!loaded)
      return;
  }

  if (birdType == BirdType::MILEI) {
    sound.setBuffer(mileiBuffer);
  } else {
    sound.setBuffer(fujimoriBuffer);
  }

  sound.play();
}
void render_bird_game(sf::RenderWindow &ventana, int level, int width,
                      int height, BirdType birdType) {
  bool arrastrando = false;
  sf::Vector2f clickOffset;
  std::vector<sf::Vector2f> trayectoria;

  Level *lev = return_level(level, width, height);
  if (!lev) {
    return;
  }

  sf::View levelView(sf::FloatRect(0, 0, 800, 600));
  sf::FloatRect levelBounds(0, 0, lev->x_bound, lev->y_bound);
  sf::Vector2f previousMousePos;
  sf::Vector2f pos_resortera(100.0f, lev->y_bound - 130.0f);

  Slingshot resortera(pos_resortera);
  Bird pajaro(birdType, pos_resortera);

  sf::RectangleShape fondo(
      {static_cast<float>(lev->x_bound), static_cast<float>(lev->y_bound)});
  fondo.setFillColor({135, 206, 235});

  // move represents 0 for the bird and 1 for the camera
  int move = 0;

  float ground_y = height - 50.0f;
  float deltaTime = 1.0f / 60.0f;

  sf::Font font;
  if (!font.loadFromFile("fonts/arial-font/arial.ttf")) {
    if (!font.loadFromFile("fonts/angrybirds-regular.ttf")) {
      throw std::runtime_error("Failed to load fonts");
    }
  }

  sf::Text instructionText(
      "Press 1-3 to change bird: 1=Default, 2=Milei, 3=Fujimori", font, 16);
  instructionText.setPosition(10, 10);
  instructionText.setFillColor(sf::Color::Black);

  while (ventana.isOpen()) {
    sf::Event evento;
    while (ventana.pollEvent(evento)) {
      if (evento.type == sf::Event::Closed)
        ventana.close();

      if (evento.type == sf::Event::KeyPressed) {
        if (evento.key.code == sf::Keyboard::Num1 ||
            evento.key.code == sf::Keyboard::Numpad1) {
          pajaro.setBirdType(BirdType::DEFAULT);
        } else if (evento.key.code == sf::Keyboard::Num2 ||
                   evento.key.code == sf::Keyboard::Numpad2) {
          pajaro.setBirdType(BirdType::MILEI);
        } else if (evento.key.code == sf::Keyboard::Num3 ||
                   evento.key.code == sf::Keyboard::Numpad3) {
          pajaro.setBirdType(BirdType::FUJIMORI);
        }
      }

      if (evento.type == sf::Event::MouseButtonPressed) {
        sf::Vector2f mousePos =
            ventana.mapPixelToCoords(sf::Mouse::getPosition(ventana));
        if (pajaro.figura.getGlobalBounds().contains(mousePos) &&
            !pajaro.lanzado) {
          arrastrando = true;
          clickOffset = pajaro.figura.getPosition() - mousePos;
          move = 0;
        } else if (evento.mouseButton.button == sf::Mouse::Left) {
          arrastrando = true;

          previousMousePos = ventana.mapPixelToCoords(
              sf::Vector2i(evento.mouseButton.x, evento.mouseButton.y),
              levelView);
          move = 1;
        }
      }

      if (evento.type == sf::Event::MouseButtonReleased && arrastrando) {
        arrastrando = false;
        if (move == 0) {
          pajaro.lanzado = true;

          pajaro.updateTextureState();

          sf::Vector2f direccion = pos_resortera - pajaro.figura.getPosition();
          pajaro.velocidad = direccion * FUERZA_MULTIPLICADOR;
          // sound
          playBirdSound(pajaro.getBirdType());
        }
      }
    }

    if (arrastrando && move == 0) {
      sf::Vector2f mousePos =
          ventana.mapPixelToCoords(sf::Mouse::getPosition(ventana));
      sf::Vector2f nuevaPos = mousePos + clickOffset;

      sf::Vector2f vectorResortera = nuevaPos - pos_resortera;
      float distancia = std::hypot(vectorResortera.x, vectorResortera.y);

      if (distancia > RADIO_MAX_RESORTERA) {
        vectorResortera = (vectorResortera / distancia) * RADIO_MAX_RESORTERA;
        nuevaPos = pos_resortera + vectorResortera;
      }

      pajaro.figura.setPosition(nuevaPos);
      if (pajaro.useSprite) {
        pajaro.sprite.setPosition(nuevaPos);
      }

      try {
        trayectoria = Physics::calcularTrayectoria(
            nuevaPos, (pos_resortera - nuevaPos) * FUERZA_MULTIPLICADOR,
            TRAJECTORY_STEP, GRAVITY);
      } catch (...) {
        trayectoria.clear();
      }
    } else if (arrastrando && move == 1) {
      sf::Vector2f mousePos = ventana.mapPixelToCoords(
          sf::Vector2i(evento.mouseMove.x, evento.mouseMove.y), levelView);

      sf::Vector2f delta = previousMousePos - mousePos;

      levelView.move(delta);

      // --- Apply View Limits ---
      sf::Vector2f viewCenter = levelView.getCenter();
      sf::Vector2f viewSize = levelView.getSize();

      float halfViewWidth = viewSize.x / 2.0f;
      float halfViewHeight = viewSize.y / 2.0f;

      // Clamp the view's center within the level bounds
      // Ensure the left edge of the view is not less than the left edge of
      // the level
      if (viewCenter.x - halfViewWidth < levelBounds.left) {
        viewCenter.x = levelBounds.left + halfViewWidth;
      }
      // Ensure the right edge of the view is not greater than the right
      // edge of the level
      if (viewCenter.x + halfViewWidth > levelBounds.left + levelBounds.width) {
        viewCenter.x = levelBounds.left + levelBounds.width - halfViewWidth;
      }
      // Ensure the top edge of the view is not less than the top edge of
      // the level
      if (viewCenter.y - halfViewHeight < levelBounds.top) {
        viewCenter.y = levelBounds.top + halfViewHeight;
      }
      // Ensure the bottom edge of the view is not greater than the bottom
      // edge of the level
      if (viewCenter.y + halfViewHeight >
          levelBounds.top + levelBounds.height) {
        viewCenter.y = levelBounds.top + levelBounds.height - halfViewHeight;
      }

      // Set the clamped center back to the view
      levelView.setCenter(viewCenter);

      // Update lastMousePos for the next frame (important for smooth
      // dragging) We re-map it because the view might have been clamped
      previousMousePos = ventana.mapPixelToCoords(
          sf::Vector2i(evento.mouseMove.x, evento.mouseMove.y), levelView);
    }

    if (pajaro.lanzado) {
      pajaro.updatePhysics(deltaTime);

      if (pajaro.figura.getPosition().y + pajaro.figura.getRadius() >
          ground_y) {
        Physics::handleGroundCollision(pajaro.figura, pajaro.velocidad,
                                       ground_y);
        if (pajaro.useSprite) {
          pajaro.sprite.setPosition(pajaro.figura.getPosition());
        }
      }

      if (std::abs(pajaro.velocidad.x) < 15.0f &&
          std::abs(pajaro.velocidad.y) < 15.0f &&
          pajaro.figura.getPosition().y + pajaro.figura.getRadius() >=
              ground_y - 0.1f) {
        pajaro.reset();
        arrastrando = false; // Reset arrastrando flag
      }

      if (pajaro.figura.getPosition().x < -100.0f ||
          pajaro.figura.getPosition().x > width + 100.0f ||
          pajaro.figura.getPosition().y > height + 100.0f) {
        pajaro.reset();
        arrastrando = false; // Reset arrastrando flag
      }
    }

    ventana.clear();

    ventana.setView(levelView);

    ventana.draw(fondo);
    resortera.draw(ventana);
    if (lev) {
      lev->render(ventana);
    }

    if (arrastrando || !pajaro.lanzado) {
      resortera.updateBands(pajaro.figura.getPosition(), arrastrando);
    }

    if (arrastrando && !trayectoria.empty() && move == 0) {
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

    pajaro.draw(ventana);
    ventana.draw(instructionText);
    ventana.display();
  }

  delete lev;
}
