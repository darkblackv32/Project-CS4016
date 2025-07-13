#pragma once

#include "PhysicsWrapper.h"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/ConvexShape.hpp>
#include <SFML/System/Vector2.hpp>
#include <memory>
#include <utility>
#include <vector>

// Enum para los tipos de figuras geométricas usando SFML shapes
enum class SFMLShapeType {
    RECTANGLE,
    CIRCLE,
    TRIANGLE,
    DIAMOND,
    HEXAGON,
    PENTAGON,
    CUSTOM_POLYGON
};

struct Level {
  std::vector<std::unique_ptr<sf::Shape>> objects;
  std::vector<sf::Texture> target_textures;
  std::vector<std::unique_ptr<sf::Shape>> targets;
  std::vector<std::unique_ptr<sf::Shape>> floor;

  std::vector<b2Body *> m_bodies;
  std::vector<b2Body *> m_targets;
  std::vector<b2Body *> m_static;

  float START_LEVEL_X;
  float START_LEVEL_Y;

  int x_bound;
  int y_bound;

  int n_level;

  PhysicsWrapper m_physics;

  Level();
  ~Level();

  void setObjects(std::vector<sf::Vector2f> &objectSizes,
                  std::vector<std::pair<float, float>> &objectPos,
                  std::vector<sf::Color> &objectColors,
                  const std::vector<SFMLShapeType> &shapeTypes = {});

  void setTargets(std::vector<sf::Vector2f> &objectSizes,
                  std::vector<std::pair<float, float>> &objectPos,
                  std::vector<sf::Color> &objectColors,
                  const std::vector<std::string> &texturePaths,
                  const std::vector<SFMLShapeType> &shapeTypes = {});
  void setFloor(std::vector<sf::Vector2f> &objectSizes,
                std::vector<std::pair<float, float>> &objectPos,
                std::vector<sf::Color> &objectColors,
                const std::vector<SFMLShapeType> &shapeTypes = {});

  void setStarts(float x, float y);
  void setNLevel(int n);

  void setBounds(float x, float y);

  b2Body *createBox(float x, float y, float halfWidth, float halfHeight);
  b2Body *createCircle(float x, float y, float radius);
  b2Body *createBird(const sf::Vector2f &pos, float radius);

  // Factory methods para crear diferentes tipos de shapes SFML
  std::unique_ptr<sf::Shape> createSFMLShape(SFMLShapeType type, 
                                             const sf::Vector2f& size, 
                                             const sf::Vector2f& position, 
                                             const sf::Color& color);
  std::unique_ptr<sf::ConvexShape> createTriangle(const sf::Vector2f& size, 
                                                  const sf::Vector2f& position);
  std::unique_ptr<sf::ConvexShape> createDiamond(const sf::Vector2f& size, 
                                                 const sf::Vector2f& position);
  std::unique_ptr<sf::ConvexShape> createHexagon(const sf::Vector2f& size, 
                                                 const sf::Vector2f& position);
  std::unique_ptr<sf::ConvexShape> createPentagon(const sf::Vector2f& size, 
                                                  const sf::Vector2f& position);
  std::unique_ptr<sf::ConvexShape> createCustomPolygon(const std::vector<sf::Vector2f>& vertices,
                                                        const sf::Vector2f& position);

  void render(sf::RenderWindow &ventana);
  void run(float deltaTime);
  int over();

  void add_efect_bird(b2Body *bird_body);

  void update();
};

struct LevelPreview {
  std::string title;
  sf::Text text;
  sf::Text description;
  std::shared_ptr<sf::Font> font;
  std::shared_ptr<sf::Texture> texture;
  sf::Sprite sprite;
};

Level *return_level(int level, int width, int height);
LevelPreview get_level_preview(int level);

void test_level(int level);
