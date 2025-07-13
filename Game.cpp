#include "Game.h"

#include <SFML/Window/Keyboard.hpp>
#include <iostream>

#include "Bird.h"
#include "Constants.h"
#include "Levels.h"
#include "Particles.h"
#include "Pause.h"
#include "PhysicsWrapper.h"
#include "Slingshot.h"
#include "Victory.h"
#include "helper.h"
#include <vector>

#include <SFML/Audio.hpp>

const float TRAJECTORY_STEP = 0.0001f;

void move_view(sf::View &levelView, sf::FloatRect &levelBounds) {

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
  if (viewCenter.y + halfViewHeight > levelBounds.top + levelBounds.height) {
    viewCenter.y = levelBounds.top + levelBounds.height - halfViewHeight;
  }

  // Set the clamped center back to the view
  levelView.setCenter(viewCenter);
}

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
  sf::Vector2f clickOffset;
  std::vector<sf::Vector2f> trayectoria;

  Level *lev = return_level(level, width, height);
  if (!lev) {
    return -1;
  }

  sf::View levelView(sf::FloatRect(0, 0, lev->x_bound, lev->y_bound));
  sf::FloatRect levelBounds(0, 0, lev->x_bound, lev->y_bound);
  sf::Vector2f previousMousePos;
  sf::Vector2f pos_resortera(150.0f, lev->y_bound - 5.5 * BLOCK);
  levelView.setCenter(lev->x_bound / 2, lev->y_bound / 2);
  // move represents 0 for the bird and 1 for the camera
  ventana.setView(levelView);
  int move = 0;

  // For the pause/victory menu
  int response = 0;

  // For the particle system
  ParticleSystem particles;

  sf::Texture backgroundTexture;
  std::string backgroundPath;
  switch (level) {
  case 0:
  case 1:
    backgroundPath = "assets/textures/city1.png";
    break;
  case 2:
    backgroundPath = "assets/textures/city2.jpg";
    break;
  case 3:
    backgroundPath = "assets/textures/palacio_gobierno.jpg";
    break;
  default:
    // Optional: a default background if level is out of range
    backgroundPath = "assets/textures/palacio_gobierno.jpg";
    break;
  }

  if (!backgroundTexture.loadFromFile(backgroundPath)) {
    throw std::runtime_error("Failed to load background texture for level " +
                             std::to_string(level));
  }

  sf::RectangleShape fondo(
      {static_cast<float>(lev->x_bound), static_cast<float>(lev->y_bound)});
  fondo.setTexture(&backgroundTexture);

  // IMPORTANT
  // TODO
  // FIx scaling from the texture. Currently, the texture goes over the circle
  Bird pajaro(birdType, pos_resortera);
  Slingshot resortera(pos_resortera);

  float deltaTime = 1.0f / 60.0f;

  // Used to save the the thrown bird
  b2Body *bird_body = nullptr;

  // bird's abilty
  bool used = 0;

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
        if (evento.key.code == sf::Keyboard::Num2 ||
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
        sf::Vector2f mousePos =
            ventana.mapPixelToCoords(sf::Mouse::getPosition(ventana));
        if (pajaro.lanzado && used == 0) {
          // This applies a dwon force making the stop condition of velocity
          // less certain
          lev->add_efect_bird(bird_body);
          used = 1;
        } else if (pajaro.figura.getGlobalBounds().contains(mousePos) &&
                   !pajaro.lanzado) {
          arrastrando = true;
          previousMousePos = ventana.mapPixelToCoords(
              sf::Vector2i(evento.mouseButton.x, evento.mouseButton.y),
              levelView);
          move = 0;
          used = 0;
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
          bird_body = lev->createBird(pajaro.figura.getPosition(),
                                      pajaro.figura.getRadius());
          bird_body->SetAwake(true);

          sf::Vector2f dragEndPos = ventana.mapPixelToCoords(
              {evento.mouseButton.x, evento.mouseButton.y});
          sf::Vector2f launchVector = previousMousePos - dragEndPos;

          float launchPower = 0.5f;
          bird_body->SetLinearVelocity(b2Vec2(launchVector.x * launchPower,
                                              launchVector.y * launchPower));

          // Create texture
          pajaro.updateTextureState();

          // sound
          playBirdSound(pajaro.getBirdType());
        }
      }
    }

    if (response > 1) {
      break;
    }

    if (arrastrando && move == 0) {
      // This logic of only updating the view of the circle isn't moving the
      // circle in the physics engine Will still have the impact, but the
      // starting position is other
      // TODO
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
      if (pajaro.sprite.getTexture()) {
        pajaro.sprite.setPosition(nuevaPos);
      }

    } else if (arrastrando && move == 1) {
      sf::Vector2f mousePos =
          ventana.mapPixelToCoords(sf::Mouse::getPosition(ventana), levelView);

      sf::Vector2f delta = previousMousePos - mousePos;

      levelView.move(delta);
      move_view(levelView, levelBounds);

      // Update lastMousePos for the next frame (important for smooth
      // dragging) We re-map it because the view might have been clamped
      previousMousePos =
          ventana.mapPixelToCoords(sf::Mouse::getPosition(ventana), levelView);
    }

    // Actualizacion de fisica
    float deltaTime = deltaClock.restart().asSeconds();
    lev->run(deltaTime);
    // Iterate through the bodies marked for destruction by the PhysicsWrapper
    for (b2Body *bodyToDestroy : lev->m_physics.toDestroy) {
      // Find the index by searching
      sf::Vector2f destructionPos =
          metersToPixels(bodyToDestroy->GetPosition());
      particles.emit(destructionPos, 50);
      bool deleted = false;
      for (size_t i = 0; i < lev->m_bodies.size(); ++i) {
        if (lev->m_bodies[i] == bodyToDestroy) {
          // 1. Get the user data associated with the body
          bodyLife *userData =
              static_cast<bodyLife *>(lev->m_bodies[i]->GetUserData());

          // 2. Delete the SFML shape
          lev->objects.erase(lev->objects.begin() +
                             i); // Remove from shapes vector

          // 3. Delete the custom user data
          delete userData;

          // 4. Destroy the Box2D body
          // Calls m_world->DestroyBody()
          lev->m_physics.DestroyBody(lev->m_bodies[i]);
          lev->m_bodies.erase(lev->m_bodies.begin() + i);
          deleted = true;
          break;
        }
      }
      if (!deleted) {
        std::cout << "Is a target" << std::endl;
        for (size_t i = 0; i < lev->m_targets.size(); ++i) {
          if (lev->m_targets[i] == bodyToDestroy) {
            // 1. Get the user data associated with the body
            bodyLife *userData =
                static_cast<bodyLife *>(lev->m_targets[i]->GetUserData());

            // 2. Delete the SFML shape
            lev->targets.erase(lev->targets.begin() +
                               i); // Remove from shapes vector

            // 3. Delete the custom user data
            delete userData;

            // 4. Destroy the Box2D body
            // Calls m_world->DestroyBody()
            lev->m_physics.DestroyBody(lev->m_targets[i]);
            lev->m_targets.erase(lev->m_targets.begin() + i);
            deleted = true;
            break;
          }
        }
      }
    }
    lev->m_physics.toDestroy.clear();

    // update particles
    particles.update(deltaTime);

    // TODO
    // Also, when things dissapear, implement particle effects

    if (pajaro.lanzado) {
      // Run simulation
      lev->run(deltaTime);
      // Update in sfml
      b2Fixture *fixture = bird_body->GetFixtureList();
      b2Shape::Type shapeType = fixture->GetType();
      sf::Vector2f pos = metersToPixels(bird_body->GetPosition());
      float angle = bird_body->GetAngle() * 180.f / b2_pi;
      pajaro.figura.setPosition(pos);

      pajaro.sprite.setPosition(pajaro.figura.getPosition());

      // Resets when the bird is done
      // Done means out of bounds or too slow. Other way in place of being too
      // slow could be used, like comparing x number of previous positions and
      // see the average difference to prevent the reset from being too sudden
      b2Vec2 velocity = bird_body->GetLinearVelocity();
      if (pajaro.figura.getPosition().x < -100.0f ||
          pajaro.figura.getPosition().x > lev->x_bound + 100.0f ||
          pajaro.figura.getPosition().y > lev->y_bound + 100.0f ||
          pajaro.figura.getPosition().y < -300.0f ||
          (abs(velocity.x) < THRESHOLD_VELOCITY &&
           abs(velocity.y) < THRESHOLD_VELOCITY)) {
        // removes the bird from the physics engine
        lev->m_physics.DestroyBody(bird_body);
        // resets the variable
        pajaro.reset();
      }

      // updates the view to follow the bird
      levelView.setCenter(pajaro.figura.getPosition().x,
                          pajaro.figura.getPosition().y);
      move_view(levelView, levelBounds);
    }

    if (lev->targets.size() == 0) {
      break;
    }

    ventana.clear();

    ventana.setView(levelView);

    ventana.draw(fondo);

    // renders the objects and update them according to the bodies
    if (lev) {
      lev->render(ventana);
    }

    if ((arrastrando || !pajaro.lanzado) && move == 0) {
      resortera.updateBands(pajaro.figura.getPosition(), arrastrando);
    }
    resortera.draw(ventana);

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

    // Draw particles
    particles.draw(ventana);

    // ventana.draw(instructionText);
    ventana.display();
  }

  if (response == 0) {
    response = victory_screen(ventana, fondo, lev, resortera, pajaro);
  }

  delete lev;
  return response;
}
