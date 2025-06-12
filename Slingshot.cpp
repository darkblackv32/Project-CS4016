#include "Slingshot.h"

Slingshot::Slingshot(sf::Vector2f &pos_resortera) {
  loadTextures();

  posteIzq.setSize({30.0f, 220.0f});
  posteIzq.setTexture(&woodTexture);
  posteIzq.setOrigin(5.0f, 0.0f);
  posteIzq.setPosition(pos_resortera.x - 55.0f, pos_resortera.y - 100.0f);
  posteIzq.setRotation(-5.0f);

  posteDer.setSize({30.0f, 220.0f});
  posteDer.setTexture(&woodTexture);
  posteDer.setOrigin(25.0f, 0.0f);
  posteDer.setPosition(pos_resortera.x + 55.0f, pos_resortera.y - 100.0f);
  posteDer.setRotation(5.0f);

  bandaIzq.setPrimitiveType(sf::TrianglesStrip);
  bandaIzq.resize(4);
  bandaDer.setPrimitiveType(sf::TrianglesStrip);
  bandaDer.resize(4);
}

void Slingshot::loadTextures() {
  woodTexture.create(32, 32);
  sf::Uint8 *pixels = new sf::Uint8[32 * 32 * 4];
  for (int y = 0; y < 32; ++y) {
    for (int x = 0; x < 32; ++x) {
      int index = (y * 32 + x) * 4;
      pixels[index] = 139;     // R
      pixels[index + 1] = 69;  // G
      pixels[index + 2] = 19;  // B
      pixels[index + 3] = 255; // A
      if (x % 4 == 0) {
        pixels[index] += 20;
        pixels[index + 1] += 10;
      }
    }
  }
  woodTexture.update(pixels);
  delete[] pixels;
}

void Slingshot::updateBands(const sf::Vector2f &birdPosition,
                            bool isStretched) {
  auto createBand = [&](const sf::Vector2f &start, const sf::Vector2f &end,
                        sf::VertexArray &va) {
    const float sag = isStretched ? 8.0f : 20.0f;

    va[0].position = start;
    va[0].color = sf::Color(80, 80, 80, 200);

    va[1].position = start + sf::Vector2f(0, 5.0f);
    va[1].color = sf::Color(80, 80, 80, 200);

    va[2].position = end;
    va[2].color = sf::Color(80, 80, 80, 200);

    va[3].position = end + sf::Vector2f(0, 5.0f);
    va[3].color = sf::Color(80, 80, 80, 200);
  };

  createBand(posteIzq.getPosition() + sf::Vector2f(15.0f, 60.0f), birdPosition,
             bandaIzq);
  createBand(posteDer.getPosition() + sf::Vector2f(-15.0f, 60.0f), birdPosition,
             bandaDer);
}

void Slingshot::draw(sf::RenderWindow &window) {
  window.draw(posteIzq);
  window.draw(posteDer);
  window.draw(bandaIzq);
  window.draw(bandaDer);
}
