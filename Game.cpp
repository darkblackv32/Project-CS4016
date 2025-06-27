#include "Game.h"

#include <SFML/Window/Keyboard.hpp>
#include <iostream>

#include "Bird.h"
#include "Constants.h"
#include "Levels.h"
#include "Pause.h"
#include "Physics.h"
#include "Slingshot.h"
#include "helper.h"
#include "polyphysics.h"
#include <vector>

#include <SFML/Audio.hpp>

const float TRAJECTORY_STEP = 0.0001f;

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

int render_bird_game(sf::RenderWindow &ventana, int level, int width,
                     int height, BirdType birdType) {
  bool arrastrando = false;
  std::vector<sf::Vector2f> trayectoria;

  Level *lev = return_level(level, width, height);
  if (!lev) {
    return -1;
  }

  // this is set the the ventana size, so this could cause some problems
  // Currently it matches the width of the window
  sf::View levelView(sf::FloatRect(0, 0, 800, ventana.getSize().y));
  std::cout << ventana.getSize().x << ", " << ventana.getSize().y << std::endl;
  sf::FloatRect levelBounds(0, 0, lev->x_bound, lev->y_bound);
  sf::Vector2f previousMousePos;
  sf::Vector2f clickOffset;
  sf::Vector2f pos_resortera(100.0f, lev->y_bound - 200.0f);
  // move represents 0 for the bird and 1 for the camera
  levelView.setCenter(lev->x_bound / 2.0f, lev->y_bound / 2.0f);
  int move = -1;

  // For the pause menu
  int response = 1;

  sf::Texture backgroundTexture;
  if (!backgroundTexture.loadFromFile("assets/textures/palacio_gobierno.jpg")) {
    throw std::runtime_error("Failed to load background texture");
  }

  sf::RectangleShape fondo(
      {static_cast<float>(lev->x_bound), static_cast<float>(lev->y_bound)});
  fondo.setTexture(&backgroundTexture);

  Bird pajaro(birdType, pos_resortera);
  Slingshot resortera(pos_resortera);

  // initializatin for physics for the bird.
  auto bird_original_pos = {pajaro.figura.getPosition().x,
                            pajaro.figura.getPosition().y};
  int birdId = 0;
  birdId = lev->physicsEngine.addBody(
      {pajaro.figura.getPosition().x, pajaro.figura.getPosition().y},
      createCircleCollider(pajaro.figura.getRadius()));
  lev->physicsEngine.bodies[birdId].elasticity = 0.5f;
  // Hacemos al pájaro más pesado
  lev->physicsEngine.bodies[birdId].motionState.mass = 20.f;
  // Lo hacemos cinemático hasta que se lance
  lev->physicsEngine.bodies[birdId].flags.setKinematic(true);

  sf::Font font;
  if (!font.loadFromFile("assets/fonts/arial-font/arial.ttf")) {
    if (!font.loadFromFile("assets/fonts/angrybirds-regular.ttf")) {
      throw std::runtime_error("Failed to load fonts");
    }
  }

  sf::Text instructionText(
      "Press 1-3 to change bird: 1=Default, 2=Milei, 3=Fujimori", font, 16);
  instructionText.setPosition(10, 10);
  instructionText.setFillColor(sf::Color::Black);

  // Reloj para medir el tiempo delta (deltaTime)
  sf::Clock deltaClock;

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
        } else if (evento.key.code == sf::Keyboard::Escape) {
          response = render_pause_menu(ventana);
        }
      }

      if (evento.type == sf::Event::MouseButtonPressed &&
          evento.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2f mousePos = ventana.mapPixelToCoords(
            sf::Vector2i(evento.mouseButton.x, evento.mouseButton.y),
            levelView);

        sf::Vector2f mousePosOld =
            ventana.mapPixelToCoords(sf::Mouse::getPosition(ventana));

        if (pajaro.figura.getGlobalBounds().contains(mousePos) &&
            lev->physicsEngine.bodies[birdId].flags.isKinematic()) {
          arrastrando = true;
          previousMousePos = mousePos;
          clickOffset = pajaro.figura.getPosition() - mousePos;
          move = 0;
        } else {
          arrastrando = true;
          previousMousePos = mousePos;
          move = 1;
        }
      }

      if (evento.type == sf::Event::MouseButtonReleased &&
          evento.mouseButton.button == sf::Mouse::Left && arrastrando) {
        arrastrando = false;
        if (move == 0) {
          std::cout << "Activate pajaro lanzado" << std::endl;

          sf::Vector2f dragEndPos = ventana.mapPixelToCoords(
              sf::Vector2i(evento.mouseButton.x, evento.mouseButton.y),
              levelView);
          sf::Vector2f dragVector = sf::Vector2f(previousMousePos - dragEndPos);

          std::cout << "drag vector: " << dragVector.x << ", " << dragVector.y
                    << std::endl;

          // Change physics to work
          // Works, however it's rather slow. Play with the factor, to make it
          // go fast. Move the resortera to be higher too, so that we can make
          // use of the full power of resortera. Make the bird smaller perhaps
          // Moves instantly too sometimes so something isn't updated on time
          // All of the building falls together. The floor be dammed
          // TODO
          float factor = 4.f;
          lev->physicsEngine.bodies[birdId].flags.setKinematic(false);
          lev->physicsEngine.bodies[birdId].motionState.velocity =
              fromSfmlVec(dragVector) * factor;

          std::cout << "First velocity: "
                    << lev->physicsEngine.bodies[birdId].motionState.velocity.x
                    << ", "
                    << lev->physicsEngine.bodies[birdId].motionState.velocity.y
                    << std::endl;

          pajaro.updateTextureState();

          // sound
          playBirdSound(pajaro.getBirdType());

          pajaro.lanzado = true;
        }
      }
    }

    if (response > 1) {
      break;
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
      lev->physicsEngine.bodies[birdId].motionState.setPos(
          fromSfmlVec(nuevaPos));

      if (pajaro.sprite.getTexture()) {
        pajaro.sprite.setPosition(nuevaPos);
      }

      try {
        // Fix to work according to the new physics
        // TODO
        trayectoria = Physics::calcularTrayectoria(
            nuevaPos, (pos_resortera - nuevaPos) * FUERZA_MULTIPLICADOR,
            TRAJECTORY_STEP, GRAVEDAD);
      } catch (...) {
        trayectoria.clear();
      }
    } else if (arrastrando && move == 1) {
      sf::Vector2f mousePos =
          ventana.mapPixelToCoords(sf::Mouse::getPosition(ventana), levelView);

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
      previousMousePos =
          ventana.mapPixelToCoords(sf::Mouse::getPosition(ventana), levelView);
    }

    // --- ACTUALIZACIÓN DE LA FÍSICA ---
    float deltaTime = deltaClock.restart().asSeconds();
    lev->run(deltaTime);

    if (pajaro.lanzado) {
      pajaro.figura.setPosition(
          lev->physicsEngine.bodies[birdId].motionState.pos.x,
          lev->physicsEngine.bodies[birdId].motionState.pos.y);

      pajaro.sprite.setPosition(pajaro.figura.getPosition());

      std::cout << "Velocity: "
                << lev->physicsEngine.bodies[birdId].motionState.velocity.x
                << ", "
                << lev->physicsEngine.bodies[birdId].motionState.velocity.y
                << std::endl;
      std::cout << "Acceleration: "
                << lev->physicsEngine.bodies[birdId].motionState.acceleration.x
                << ", "
                << lev->physicsEngine.bodies[birdId].motionState.acceleration.y
                << std::endl;
      std::cout << "Pos: "
                << lev->physicsEngine.bodies[birdId].motionState.pos.x << ", "
                << lev->physicsEngine.bodies[birdId].motionState.pos.y
                << std::endl;

      // Also considers if the bird goes beyond the limits
      if ((lev->physicsEngine.bodies[birdId].motionState.velocity.x <
               THRESHOLD_STOP &&
           lev->physicsEngine.bodies[birdId].motionState.velocity.y <
               THRESHOLD_STOP) ||
          pajaro.figura.getPosition().x < -100.0f ||
          pajaro.figura.getPosition().x > lev->x_bound + 100.0f ||
          pajaro.figura.getPosition().y > lev->y_bound + 100.0f) {
        // resets the variable
        pajaro.reset();
        // removes the bird from the physics engine and removes the bird id
        lev->physicsEngine.bodies[birdId].motionState.setPos(
            fromSfmlVec(pajaro.figura.getPosition()));
      }

      // --- LÓGICA DEL JUEGO (POST-FÍSICA) ---
      // Implementación de la reacción en cadena

      std::vector<ph2dBodyId> bodiesToWakeUp;
      for (const auto &manifold : lev->physicsEngine.intersections) {
        Body &bodyA = lev->physicsEngine.bodies[manifold.A];
        Body &bodyB = lev->physicsEngine.bodies[manifold.B];

        // Si un cuerpo dinámico choca con uno cinemático de la estructura, lo
        // despertamos.
        if (!bodyA.flags.isKinematic() && bodyB.flags.isKinematic()) {
          for (auto structureId : lev->object_ids) {
            if (manifold.B == structureId) {
              bodiesToWakeUp.push_back(manifold.B);
              break;
            }
          }
        }
        if (!bodyB.flags.isKinematic() && bodyA.flags.isKinematic()) {
          for (auto structureId : lev->object_ids) {
            if (manifold.A == structureId) {
              bodiesToWakeUp.push_back(manifold.A);
              break;
            }
          }
        }
      }

      // Activamos todos los cuerpos que deben despertar
      for (auto id : bodiesToWakeUp) {
        lev->physicsEngine.bodies[id].flags.setKinematic(false);
      }
    }

    ventana.clear();

    ventana.setView(levelView);

    ventana.draw(fondo);

    // renders the objects and update them according to the bodies
    if (lev) {
      lev->render(ventana);
    }
    resortera.draw(ventana);

    if ((arrastrando || !pajaro.lanzado) && move == 0) {
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
  return response;
}
