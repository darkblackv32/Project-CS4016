#include "Game.h"

#include <iostream>

#include "Bird.h"
#include "Constants.h"
#include "Levels.h"
#include "Physics.h"
#include "Slingshot.h"
#include <vector>

#include <SFML/Audio.hpp>

const float TRAJECTORY_STEP = 0.0001f;
const float GRAVITY = 0.08f;


void playBirdSound(BirdType birdType) {
    static sf::SoundBuffer mileiBuffer;
    static sf::SoundBuffer fujimoriBuffer;
    static bool buffersLoaded = false;
    static sf::Sound sound;

    if (!buffersLoaded) {
        bool loaded = true;
        if (!mileiBuffer.loadFromFile("assets/audios/milei.ogg")) {
            std::cerr << "Failed to load audios/milei.ogg" << std::endl;
            loaded = false;
        }
        if (!fujimoriBuffer.loadFromFile("assets/audios/fujimori.ogg")) {
            std::cerr << "Failed to load audios/fujimori.ogg" << std::endl;
            loaded = false;
        }
        buffersLoaded = loaded;
        if (!loaded) return;
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
  Bird pajaro(birdType);
  Slingshot resortera;
  bool arrastrando = false;
  sf::Vector2f clickOffset;
  std::vector<sf::Vector2f> trayectoria;

  sf::RectangleShape fondo({static_cast<float>(width), static_cast<float>(height)});
  fondo.setFillColor({135, 206, 235});

  Level *lev = return_level(level, width, height);
  if (!lev) {
    return;
  }

  float ground_y = height - 50.0f;
  float deltaTime = 1.0f / 60.0f;

  sf::Font font;
  if (!font.loadFromFile("assets/fonts/arial-font/arial.ttf")) {
    if (!font.loadFromFile("assets/fonts/angrybirds-regular.ttf")) {
      throw std::runtime_error("Failed to load fonts");
    }
  }

  sf::Text instructionText("Press 1-3 to change bird: 1=Default, 2=Milei, 3=Fujimori", font, 16);
  instructionText.setPosition(10, 10);
  instructionText.setFillColor(sf::Color::Black);

  while (ventana.isOpen()) {
    sf::Event evento;
    while (ventana.pollEvent(evento)) {
      if (evento.type == sf::Event::Closed)
        ventana.close();

      if (evento.type == sf::Event::KeyPressed) {
        if (evento.key.code == sf::Keyboard::Num1 || evento.key.code == sf::Keyboard::Numpad1) {
          pajaro.setBirdType(BirdType::DEFAULT);
        } else if (evento.key.code == sf::Keyboard::Num2 || evento.key.code == sf::Keyboard::Numpad2) {
          pajaro.setBirdType(BirdType::MILEI);
        } else if (evento.key.code == sf::Keyboard::Num3 || evento.key.code == sf::Keyboard::Numpad3) {
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
        }
      }

      if (evento.type == sf::Event::MouseButtonReleased && arrastrando) {
        arrastrando = false;
        pajaro.lanzado = true;

        pajaro.updateTextureState();

        sf::Vector2f direccion = POS_RESORTERA - pajaro.figura.getPosition();
        pajaro.velocidad = direccion * FUERZA_MULTIPLICADOR;
        // sound
        playBirdSound(pajaro.getBirdType());
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
      if (pajaro.sprite.getTexture()) {
        pajaro.sprite.setPosition(nuevaPos);
      }

      try {
        trayectoria = Physics::calcularTrayectoria(
            nuevaPos, (POS_RESORTERA - nuevaPos) * FUERZA_MULTIPLICADOR,
            TRAJECTORY_STEP,
            GRAVITY
        );
      } catch (...) {
        trayectoria.clear();
      }
    }

    if (pajaro.lanzado) {
      pajaro.updatePhysics(deltaTime);

      if (pajaro.figura.getPosition().y + pajaro.figura.getRadius() > ground_y) {
        Physics::handleGroundCollision(pajaro.figura, pajaro.velocidad, ground_y);
        if (pajaro.sprite.getTexture()) {
          pajaro.sprite.setPosition(pajaro.figura.getPosition());
        }
      }

      if (std::abs(pajaro.velocidad.x) < 15.0f &&
          std::abs(pajaro.velocidad.y) < 15.0f &&
          pajaro.figura.getPosition().y + pajaro.figura.getRadius() >=
              ground_y - 0.1f) {
        pajaro.reset();
        arrastrando = false; // reset arrastrando flag
      }

      if (pajaro.figura.getPosition().x < -100.0f ||
          pajaro.figura.getPosition().x > width + 100.0f ||
          pajaro.figura.getPosition().y > height + 100.0f) {
        pajaro.reset();
        arrastrando = false; // Reset arrastrando flag
      }
    }

    ventana.clear();

    ventana.draw(fondo);
    resortera.draw(ventana);
    if (lev) {
      lev->render(ventana);
    }

    if (arrastrando || !pajaro.lanzado) {
      resortera.updateBands(pajaro.figura.getPosition(), arrastrando);
    }

    if (arrastrando && !trayectoria.empty()) {
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