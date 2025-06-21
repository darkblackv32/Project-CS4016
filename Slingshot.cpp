#include "Slingshot.h"

namespace {
const sf::Color WOOD_COLOR(139, 69, 19);
const sf::Color BAND_COLOR(80, 80, 80, 200);
const sf::Vector2f POST_OFFSET(-55.0f, -100.0f);
const sf::Vector2f LEFT_ORIGIN(5.0f, 0.0f);
const sf::Vector2f RIGHT_ORIGIN(25.0f, 0.0f);
const sf::Vector2f BAND_ATTACH_LEFT(15.0f, 60.0f);
const sf::Vector2f BAND_ATTACH_RIGHT(-15.0f, 60.0f);
} // namespace

Slingshot::Slingshot(sf::Vector2f &pr) {
  leftPost.setSize({POST_WIDTH, POST_HEIGHT});
  rightPost.setSize({POST_WIDTH, POST_HEIGHT});

  leftBand.setPrimitiveType(sf::TriangleStrip);
  rightBand.setPrimitiveType(sf::TriangleStrip);

  this->pos_resortera = pr;

  configurePosts();
  configureBands();
}

sf::Texture Slingshot::createWoodTexture() {
  sf::Texture texture;
  texture.create(32, 32);

  std::vector<sf::Uint8> pixels(32 * 32 * 4);
  for (int y = 0; y < 32; ++y) {
    for (int x = 0; x < 32; ++x) {
      const size_t index = (y * 32 + x) * 4;
      pixels[index] = WOOD_COLOR.r;
      pixels[index + 1] = WOOD_COLOR.g;
      pixels[index + 2] = WOOD_COLOR.b;
      pixels[index + 3] = 255;

      if (x % 4 == 0) {
        pixels[index] = std::min(255, pixels[index] + 20);
        pixels[index + 1] = std::min(255, pixels[index + 1] + 10);
      }
    }
  }

  texture.update(pixels.data());
  return texture;
}

void Slingshot::configurePosts() {
  static auto woodTexture = createWoodTexture();

  leftPost.setTexture(&woodTexture);
  leftPost.setOrigin(LEFT_ORIGIN);
  leftPost.setPosition(pos_resortera + POST_OFFSET);
  leftPost.setRotation(-5.0f);

  rightPost.setTexture(&woodTexture);
  rightPost.setOrigin(RIGHT_ORIGIN);
  rightPost.setPosition(pos_resortera -
                        sf::Vector2f(POST_OFFSET.x, -POST_OFFSET.y));
  rightPost.setRotation(5.0f);
}

void Slingshot::configureBands() {
  leftBand.resize(4);
  rightBand.resize(4);

  for (int i = 0; i < 4; ++i) {
    leftBand[i].color = BAND_COLOR;
    rightBand[i].color = BAND_COLOR;
  }
}

void Slingshot::updateBandGeometry(sf::VertexArray &band,
                                   const sf::Vector2f &start,
                                   const sf::Vector2f &end, bool isStretched) {
  const sf::Vector2f direction = end - start;
  const float length = std::hypot(direction.x, direction.y);
  const sf::Vector2f unitDirection =
      (length > 0) ? direction / length : sf::Vector2f();
  const sf::Vector2f normal(-unitDirection.y, unitDirection.x);
  const sf::Vector2f offset = normal * BAND_THICKNESS * 0.5f;

  const float sag = isStretched ? 0.0f : SAG_AMOUNT;
  const sf::Vector2f midPoint = (start + end) * 0.5f + sf::Vector2f(0, sag);

  band[0].position = start - offset;
  band[1].position = start + offset;
  band[2].position = midPoint - offset;
  band[3].position = midPoint + offset;
}

void Slingshot::updateBands(const sf::Vector2f &birdPosition,
                            bool isStretched) {
  const auto leftStart = leftPost.getPosition() + BAND_ATTACH_LEFT;
  const auto rightStart = rightPost.getPosition() + BAND_ATTACH_RIGHT;

  updateBandGeometry(leftBand, leftStart, birdPosition, isStretched);
  updateBandGeometry(rightBand, rightStart, birdPosition, isStretched);
}

void Slingshot::draw(sf::RenderWindow &window) const {
  window.draw(leftPost);
  window.draw(rightPost);
  window.draw(leftBand);
  window.draw(rightBand);
}
