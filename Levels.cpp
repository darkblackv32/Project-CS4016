#include "Levels.h"
#include "Constants.h"
#include "PhysicsWrapper.h"
#include "helper.h"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <iostream>
#include <utility>
#include <vector>

int WIDTH = 800;
int HEIGHT = 600;

Level::Level() : m_physics(b2Vec2(0.0f, 9.8f)) {
  // Inicializa la gravedad del nivel
}

Level::~Level() {}

void Level::setObjects(std::vector<sf::Vector2f> &objectSizes,
                       std::vector<std::pair<float, float>> &objectPos,
                       std::vector<sf::Color> &objectColors,
                       const std::vector<SFMLShapeType> &shapeTypes) {

  std::vector<std::unique_ptr<sf::Shape>> objects;
  for (int i = 0; i < objectSizes.size(); i++) {
    // Determinar tipo de shape (por defecto RECTANGLE)
    SFMLShapeType shapeType = (i < shapeTypes.size()) ? shapeTypes[i] : SFMLShapeType::RECTANGLE;
    
    // Physics - crear cuerpo físico apropiado según el tipo
    b2Body *temp_body = nullptr;
    sf::Vector2f pos;
    
    switch (shapeType) {
      case SFMLShapeType::CIRCLE: {
        float radius = objectSizes[i].x / 2.0f; // usar componente x como radio
        temp_body = createCircle(objectPos[i].first + radius, 
                                objectPos[i].second + radius, radius);
        break;
      }
      case SFMLShapeType::RECTANGLE:
      case SFMLShapeType::TRIANGLE:
      case SFMLShapeType::DIAMOND:
      case SFMLShapeType::HEXAGON:
      case SFMLShapeType::PENTAGON:
      default: {
        // Para rectángulos y otras formas, usar física de caja
        temp_body = createBox(objectPos[i].first + objectSizes[i].x / 2,
                              objectPos[i].second + objectSizes[i].y / 2,
                              objectSizes[i].x / 2, objectSizes[i].y / 2);
        break;
      }
    }
    
    if (!temp_body) continue;
    
    pos = metersToPixels(temp_body->GetPosition());
    float angle = temp_body->GetAngle() * 180.f / b2_pi;

    bodyLife *object_life = new bodyLife(OBJECT_LIFE, OBJECT_DEFENSE);
    temp_body->SetUserData(object_life);

    // SFML - crear shape apropiado
    auto shape = createSFMLShape(shapeType, objectSizes[i], pos, objectColors[i]);
    if (shape) {
      shape->setPosition(pos);
      shape->setRotation(angle);
      shape->setOutlineColor({170, 143, 24});
      shape->setOutlineThickness(LINE_THICKNESS);
      objects.push_back(std::move(shape));
    }
  }

  this->objects = std::move(objects);
}

b2Body *Level::createBox(float x, float y, float halfWidth, float halfHeight) {
  b2BodyDef bodyDef;
  bodyDef.type = b2_dynamicBody;
  bodyDef.position.Set(x / SCALE, y / SCALE);
  b2Body *boxBody = m_physics.CreateBody(&bodyDef);

  m_physics.CreateBoxFixture(boxBody, halfWidth / SCALE, halfHeight / SCALE,
                             1.0f);
  m_bodies.push_back(boxBody);

  boxBody->SetSleepingAllowed(true);
  boxBody->SetAwake(false);

  return boxBody;
}

b2Body *Level::createCircle(float x, float y, float radius) {
  b2BodyDef bodyDef;
  bodyDef.type = b2_dynamicBody;
  bodyDef.position.Set(x / SCALE, y / SCALE);
  b2Body *circleBody = m_physics.CreateBody(&bodyDef);

  b2CircleShape circleShape;
  circleShape.m_radius = radius / SCALE;
  m_physics.CreateCircleFixture(circleBody, &circleShape, 1.0f);
  m_bodies.push_back(circleBody);

  circleBody->SetSleepingAllowed(true);
  circleBody->SetAwake(false);

  return circleBody;
}

b2Body *Level::createBird(const sf::Vector2f &pos, float radius) {
  b2Body *m_birdBody = nullptr;
  b2BodyDef birdBodyDef;
  birdBodyDef.type = b2_dynamicBody;
  birdBodyDef.position.Set(pos.x / SCALE, pos.y / SCALE);
  m_birdBody = m_physics.CreateBody(&birdBodyDef);

  b2CircleShape circleShape;
  circleShape.m_radius = radius / SCALE;
  m_physics.CreateCircleFixture(m_birdBody, &circleShape, 2.0f);

  m_birdBody->SetSleepingAllowed(true);
  m_birdBody->SetAwake(false);

  return m_birdBody;
}

// Factory methods para crear diferentes tipos de shapes SFML
std::unique_ptr<sf::Shape> Level::createSFMLShape(SFMLShapeType type, 
                                                   const sf::Vector2f& size, 
                                                   const sf::Vector2f& position, 
                                                   const sf::Color& color) {
  switch (type) {
    case SFMLShapeType::RECTANGLE: {
      auto rect = std::make_unique<sf::RectangleShape>(size);
      rect->setPosition(position);
      rect->setFillColor(color);
      rect->setOrigin(size.x / 2.0f, size.y / 2.0f);
      return rect;
    }
    case SFMLShapeType::CIRCLE: {
      float radius = size.x / 2.0f; // usar componente x como radio
      auto circle = std::make_unique<sf::CircleShape>(radius);
      circle->setPosition(position);
      circle->setFillColor(color);
      circle->setOrigin(radius, radius);
      return circle;
    }
    case SFMLShapeType::TRIANGLE: {
      auto triangle = createTriangle(size, position);
      triangle->setFillColor(color);
      return triangle;
    }
    case SFMLShapeType::DIAMOND: {
      auto diamond = createDiamond(size, position);
      diamond->setFillColor(color);
      return diamond;
    }
    case SFMLShapeType::HEXAGON: {
      auto hexagon = createHexagon(size, position);
      hexagon->setFillColor(color);
      return hexagon;
    }
    case SFMLShapeType::PENTAGON: {
      auto pentagon = createPentagon(size, position);
      pentagon->setFillColor(color);
      return pentagon;
    }
    default:
      // Por defecto crear un rectángulo
      auto rect = std::make_unique<sf::RectangleShape>(size);
      rect->setPosition(position);
      rect->setFillColor(color);
      rect->setOrigin(size.x / 2.0f, size.y / 2.0f);
      return rect;
  }
}

std::unique_ptr<sf::ConvexShape> Level::createTriangle(const sf::Vector2f& size, 
                                                       const sf::Vector2f& position) {
  auto triangle = std::make_unique<sf::ConvexShape>(3);
  
  float halfWidth = size.x / 2.0f;
  float halfHeight = size.y / 2.0f;
  
  // Vértices de un triángulo equilátero
  triangle->setPoint(0, sf::Vector2f(0, -halfHeight));        // Vértice superior
  triangle->setPoint(1, sf::Vector2f(-halfWidth, halfHeight)); // Vértice inferior izquierdo
  triangle->setPoint(2, sf::Vector2f(halfWidth, halfHeight));  // Vértice inferior derecho
  
  triangle->setPosition(position);
  triangle->setOrigin(0, 0); // Centro en el origen
  
  return triangle;
}

std::unique_ptr<sf::ConvexShape> Level::createDiamond(const sf::Vector2f& size, 
                                                      const sf::Vector2f& position) {
  auto diamond = std::make_unique<sf::ConvexShape>(4);
  
  float halfWidth = size.x / 2.0f;
  float halfHeight = size.y / 2.0f;
  
  // Vértices de un diamante
  diamond->setPoint(0, sf::Vector2f(0, -halfHeight));      // Vértice superior
  diamond->setPoint(1, sf::Vector2f(halfWidth, 0));       // Vértice derecho
  diamond->setPoint(2, sf::Vector2f(0, halfHeight));      // Vértice inferior
  diamond->setPoint(3, sf::Vector2f(-halfWidth, 0));      // Vértice izquierdo
  
  diamond->setPosition(position);
  diamond->setOrigin(0, 0);
  
  return diamond;
}

std::unique_ptr<sf::ConvexShape> Level::createHexagon(const sf::Vector2f& size, 
                                                      const sf::Vector2f& position) {
  auto hexagon = std::make_unique<sf::ConvexShape>(6);
  
  float radius = size.x / 2.0f;
  
  // Vértices de un hexágono regular
  for (int i = 0; i < 6; ++i) {
    float angle = i * 60.0f * 3.14159f / 180.0f; // 60 grados en radianes
    float x = radius * std::cos(angle);
    float y = radius * std::sin(angle);
    hexagon->setPoint(i, sf::Vector2f(x, y));
  }
  
  hexagon->setPosition(position);
  hexagon->setOrigin(0, 0);
  
  return hexagon;
}

std::unique_ptr<sf::ConvexShape> Level::createPentagon(const sf::Vector2f& size, 
                                                       const sf::Vector2f& position) {
  auto pentagon = std::make_unique<sf::ConvexShape>(5);
  
  float radius = size.x / 2.0f;
  
  // Vértices de un pentágono regular
  for (int i = 0; i < 5; ++i) {
    float angle = i * 72.0f * 3.14159f / 180.0f; // 72 grados en radianes
    float x = radius * std::cos(angle);
    float y = radius * std::sin(angle);
    pentagon->setPoint(i, sf::Vector2f(x, y));
  }
  
  pentagon->setPosition(position);
  pentagon->setOrigin(0, 0);
  
  return pentagon;
}

std::unique_ptr<sf::ConvexShape> Level::createCustomPolygon(const std::vector<sf::Vector2f>& vertices,
                                                            const sf::Vector2f& position) {
  auto polygon = std::make_unique<sf::ConvexShape>(vertices.size());
  
  for (size_t i = 0; i < vertices.size(); ++i) {
    polygon->setPoint(i, vertices[i]);
  }
  
  polygon->setPosition(position);
  
  return polygon;
}

void Level::setTargets(std::vector<sf::Vector2f> &objectSizes,
                       std::vector<std::pair<float, float>> &objectPos,
                       std::vector<sf::Color> &objectColors,
                       const std::vector<std::string> &texturePaths,
                       const std::vector<SFMLShapeType> &shapeTypes) {

  std::vector<std::unique_ptr<sf::Shape>> objects;
  if (!texturePaths.empty()) {
    this->target_textures.resize(texturePaths.size());
  }
  for (int i = 0; i < objectSizes.size(); i++) {
    // Determinar tipo de shape (por defecto CIRCLE para targets)
    SFMLShapeType shapeType = (i < shapeTypes.size()) ? shapeTypes[i] : SFMLShapeType::CIRCLE;
    
    // Physics - crear cuerpo físico apropiado según el tipo
    b2Body *temp_target = nullptr;
    sf::Vector2f pos;
    
    switch (shapeType) {
      case SFMLShapeType::CIRCLE: {
        float radius = objectSizes[i].x; // usar componente x como radio
        b2BodyDef targetDef;
        targetDef.type = b2_dynamicBody;
        targetDef.position.Set((objectPos[i].first + radius) / SCALE,
                               (objectPos[i].second + radius) / SCALE);
        temp_target = m_physics.CreateBody(&targetDef);

        b2CircleShape targetShape;
        targetShape.m_radius = radius / SCALE;
        m_physics.CreateCircleFixture(temp_target, &targetShape, 0.5f);
        break;
      }
      case SFMLShapeType::RECTANGLE:
      case SFMLShapeType::TRIANGLE:
      case SFMLShapeType::DIAMOND:
      case SFMLShapeType::HEXAGON:
      case SFMLShapeType::PENTAGON:
      default: {
        // Para otras formas, usar física de caja
        b2BodyDef targetDef;
        targetDef.type = b2_dynamicBody;
        targetDef.position.Set((objectPos[i].first + objectSizes[i].x / 2) / SCALE,
                               (objectPos[i].second + objectSizes[i].y / 2) / SCALE);
        temp_target = m_physics.CreateBody(&targetDef);

        b2PolygonShape targetShape;
        targetShape.SetAsBox(objectSizes[i].x / 2.0f / SCALE, objectSizes[i].y / 2.0f / SCALE);
        temp_target->CreateFixture(&targetShape, 0.5f);
        break;
      }
    }
    
    if (!temp_target) continue;

    temp_target->SetSleepingAllowed(true);
    temp_target->SetAwake(false);

    m_targets.push_back(temp_target);

    bodyLife *object_life = new bodyLife(TARGET_LIFE, TARGET_DEFENSE);
    temp_target->SetUserData(object_life);

    // SFML - crear shape apropiado
    pos = metersToPixels(temp_target->GetPosition());
    float angle = temp_target->GetAngle() * 180.f / b2_pi;

    auto shape = createSFMLShape(shapeType, objectSizes[i], pos, objectColors[i]);
    if (shape) {
      shape->setPosition(pos);
      shape->setRotation(angle);
      
      // Aplicar textura si está disponible
      if (!texturePaths.empty() && i < texturePaths.size()) {
        if (this->target_textures[i].loadFromFile(texturePaths[i])) {
          shape->setTexture(&this->target_textures[i]);
        }
      }
      
      objects.push_back(std::move(shape));
    }
  }

  this->targets = std::move(objects);
}

void Level::setFloor(std::vector<sf::Vector2f> &objectSizes,
                     std::vector<std::pair<float, float>> &objectPos,
                     std::vector<sf::Color> &objectColors,
                     const std::vector<SFMLShapeType> &shapeTypes) {

  std::vector<std::unique_ptr<sf::Shape>> objects;
  for (int i = 0; i < objectSizes.size(); i++) {
    // Determinar tipo de shape (por defecto RECTANGLE para el suelo)
    SFMLShapeType shapeType = (i < shapeTypes.size()) ? shapeTypes[i] : SFMLShapeType::RECTANGLE;
    
    // Physics - crear cuerpo estático
    b2Body *temp_static = nullptr;
    b2BodyDef groundBodyDef;
    
    switch (shapeType) {
      case SFMLShapeType::CIRCLE: {
        float radius = objectSizes[i].x / 2.0f; // usar componente x como radio
        groundBodyDef.position.Set((objectPos[i].first + radius) / SCALE,
                                   (objectPos[i].second + radius) / SCALE);
        temp_static = m_physics.CreateBody(&groundBodyDef);
        
        b2CircleShape groundCircle;
        groundCircle.m_radius = radius / SCALE;
        temp_static->CreateFixture(&groundCircle, 0.0f);
        break;
      }
      case SFMLShapeType::RECTANGLE:
      case SFMLShapeType::TRIANGLE:
      case SFMLShapeType::DIAMOND:
      case SFMLShapeType::HEXAGON:
      case SFMLShapeType::PENTAGON:
      default: {
        // Para rectángulos y otras formas, usar física de caja
        groundBodyDef.position.Set(
            (objectPos[i].first + objectSizes[i].x / 2.0f) / SCALE,
            (objectPos[i].second + objectSizes[i].y) / SCALE);
        temp_static = m_physics.CreateBody(&groundBodyDef);
        
        b2PolygonShape groundBox;
        groundBox.SetAsBox(objectSizes[i].x / 2.f / SCALE,
                           objectSizes[i].y / SCALE);
        temp_static->CreateFixture(&groundBox, 0.0f);
        break;
      }
    }
    
    if (!temp_static) continue;
    
    m_static.push_back(temp_static);

    // SFML - crear shape apropiado
    sf::Vector2f position(objectPos[i].first, objectPos[i].second);
    auto shape = createSFMLShape(shapeType, objectSizes[i], position, objectColors[i]);
    if (shape) {
      shape->setPosition(position);
      objects.push_back(std::move(shape));
    }
  }

  this->floor = std::move(objects);
}

void Level::setStarts(float x, float y) {
  this->START_LEVEL_X = x;
  this->START_LEVEL_Y = y;
}

void Level::setNLevel(int n) { this->n_level = n; }

void Level::setBounds(float x, float y) {
  this->x_bound = x;
  this->y_bound = y;
}

void Level::render(sf::RenderWindow &ventana) {
  // Also updates their positions according to the physics bodies
  // Renders the objects, targets and floor.
  
  // Render floor shapes
  for (auto& floorShape : this->floor) {
    if (floorShape) {
      ventana.draw(*floorShape);
    }
  }

  // Render object shapes with physics updates
  for (int i = 0; i < this->m_bodies.size() && i < this->objects.size(); i++) {
    b2Body *body = m_bodies[i];
    if (body && objects[i]) {
      sf::Vector2f pos = metersToPixels(body->GetPosition());
      float angle = body->GetAngle() * 180.f / b2_pi;

      objects[i]->setPosition(pos);
      objects[i]->setRotation(angle);
      ventana.draw(*objects[i]);
    }
  }

  // vector to have the bodies outside the level
  std::vector<int> idx_remove;
  for (int i = 0; i < this->targets.size(); i++) {
    // check if outside the bounds to delete
    if (!targets[i] || i >= m_targets.size()) continue;

    b2Body *body = m_targets[i];
    sf::Vector2f pos = metersToPixels(body->GetPosition());
    if (pos.x > x_bound + 100 || pos.x < -100 || pos.y > y_bound + 100) {
      idx_remove.push_back(i);
      continue;
    }

    // Update position and rotation from physics body
    pos = metersToPixels(body->GetPosition());
    float angle = body->GetAngle() * 180.f / b2_pi;

    targets[i]->setPosition(pos);
    targets[i]->setRotation(angle);
    
    // Apply texture if available
    if (i < this->target_textures.size()) {
      targets[i]->setTexture(&this->target_textures[i]);
    }
    
    ventana.draw(*targets[i]);
  }

  int deleted_so_far = 0;
  for (auto i : idx_remove) {
    bodyLife *userData = static_cast<bodyLife *>(m_targets[i]->GetUserData());

    targets.erase(targets.begin() + i - deleted_so_far);

    delete userData;

    m_physics.DestroyBody(m_targets[i - deleted_so_far]);
    m_targets.erase(m_targets.begin() + i - deleted_so_far);

    deleted_so_far++;
  }
}

void Level::run(float deltaTime) { m_physics.Update(deltaTime); }

int Level::over() {
  return 1;

  // if (m_targetBody &&
  //     m_targetBody->GetPosition().y > (SCREEN_HEIGHT - 40.f) / SCALE) {
  //   return 1;
  // } else {
  //   return 0;
  // }
}

void Level::add_efect_bird(b2Body *bird_body) {
  b2Vec2 impulse;
  switch (this->n_level) {
  case 0:
    break;
  case 1:
    break;
  case 2:
    impulse = b2Vec2(EXTRA_IMPULSE_RIGHT, 0);
    bird_body->ApplyLinearImpulseToCenter(impulse, true);
    break;
    break;
  case 3:
    impulse = b2Vec2(0.0f, EXTRA_IMPULSE_DOWN);
    bird_body->ApplyLinearImpulseToCenter(impulse, true);
    break;
  default:
    break;
  }
}

Level *return_level(int level, int width, int height) {
  std::vector<sf::Vector2f> objSizes;
  std::vector<std::pair<float, float>> objPositions;
  std::vector<sf::Color> objColors;
  Level *l = new Level();
  l->setNLevel(level);
  float START_LEVEL_X;
  float START_LEVEL_Y;
  int bound_x;
  int bound_y;
  std::vector<std::string> texturePaths;

  switch (level) {
  case 0: {
    bound_x = 1200;
    bound_y = 700;
    START_LEVEL_X = bound_x - BLOCK * 19;
    START_LEVEL_Y = bound_y - BLOCK * 3;

    l->setStarts(START_LEVEL_X, START_LEVEL_Y);

    l->setBounds(bound_x, bound_y);

    objSizes = {
        sf::Vector2f(BLOCK, 2 * BLOCK), sf::Vector2f(6 * BLOCK, BLOCK),
        sf::Vector2f(6 * BLOCK, BLOCK), sf::Vector2f(BLOCK, 2 * BLOCK),
        sf::Vector2f(BLOCK, 7 * BLOCK), sf::Vector2f(BLOCK, 7 * BLOCK),
        sf::Vector2f(BLOCK * 6, BLOCK), sf::Vector2f(BLOCK, 7 * BLOCK),
        sf::Vector2f(BLOCK, 7 * BLOCK), sf::Vector2f(4 * BLOCK, BLOCK),
    };

    // The offset is to be slighly above each other so there aren't weird things
    // when they collide initially
    // With smaller values there isn't much difference.
    // For now it isn't really working because the objects only react when hit
    // with the ball
    // Test more later
    objPositions = {
        std::make_pair(START_LEVEL_X, START_LEVEL_Y - 2 * BLOCK - OFFSET_ABOVE),
        std::make_pair(START_LEVEL_X, START_LEVEL_Y - OFFSET_ABOVE),
        std::make_pair(START_LEVEL_X + 6 * BLOCK, START_LEVEL_Y - OFFSET_ABOVE),
        std::make_pair(START_LEVEL_X + 11 * BLOCK,
                       START_LEVEL_Y - 2 * BLOCK - OFFSET_ABOVE),
        std::make_pair(START_LEVEL_X + 3 * BLOCK,
                       START_LEVEL_Y - 7 * BLOCK - OFFSET_ABOVE),
        std::make_pair(START_LEVEL_X + 8 * BLOCK,
                       START_LEVEL_Y - 7 * BLOCK - OFFSET_ABOVE),
        std::make_pair(START_LEVEL_X + 3 * BLOCK,
                       START_LEVEL_Y - 8 * BLOCK - OFFSET_ABOVE),
        std::make_pair(START_LEVEL_X + 4 * BLOCK,
                       START_LEVEL_Y - 15 * BLOCK - OFFSET_ABOVE),
        std::make_pair(START_LEVEL_X + 7 * BLOCK,
                       START_LEVEL_Y - 15 * BLOCK - OFFSET_ABOVE),
        std::make_pair(START_LEVEL_X + 4 * BLOCK,
                       START_LEVEL_Y - 16 * BLOCK - OFFSET_ABOVE),
    };

    objColors = {
        {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {150, 50, 200},
        {150, 50, 200}, {0, 0, 0}, {200, 100, 50}, {200, 100, 50}, {0, 0, 0},
    };

    // Add shape types - some rectangles, some pentagons
    std::vector<SFMLShapeType> shapeTypes = {
        SFMLShapeType::RECTANGLE, SFMLShapeType::RECTANGLE, SFMLShapeType::RECTANGLE, 
        SFMLShapeType::RECTANGLE, SFMLShapeType::PENTAGON, SFMLShapeType::PENTAGON,
        SFMLShapeType::RECTANGLE, SFMLShapeType::PENTAGON, SFMLShapeType::PENTAGON, 
        SFMLShapeType::RECTANGLE
    };

    l->setObjects(objSizes, objPositions, objColors, shapeTypes);

    objSizes = {sf::Vector2f(bound_x, BLOCK * 2)};
    objPositions = {std::make_pair(0.0f, bound_y - 2 * BLOCK)};
    objColors = {{120, 110, 100}};

    l->setFloor(objSizes, objPositions, objColors);

    // We use the same vector for simplicity, It holds the radio
    objSizes = {sf::Vector2f(BLOCK / 2.0f, BLOCK / 2.0f)};
    objPositions = {
        std::make_pair(START_LEVEL_X + 5.5 * BLOCK, START_LEVEL_Y - BLOCK)};
    objColors = {{9, 186, 45}};
    texturePaths.push_back("./assets/textures/milei/kirchner.png");

    l->setTargets(objSizes, objPositions, objColors, texturePaths);

    break;
  }
  case 1:
    bound_x = 2000;
    bound_y = 800;
    START_LEVEL_X = bound_x - BLOCK * 27;
    START_LEVEL_Y = bound_y - BLOCK * 6;

    l->setStarts(START_LEVEL_X, START_LEVEL_Y);

    l->setBounds(bound_x, bound_y);

    objSizes = {
        // pilars
        sf::Vector2f(BLOCK, 4 * BLOCK),
        sf::Vector2f(BLOCK, 4 * BLOCK),
        sf::Vector2f(BLOCK, 4 * BLOCK),
        sf::Vector2f(BLOCK, 4 * BLOCK),
        sf::Vector2f(BLOCK, 4 * BLOCK),
        sf::Vector2f(BLOCK, 4 * BLOCK),
        sf::Vector2f(BLOCK, 4 * BLOCK),
        sf::Vector2f(BLOCK, 4 * BLOCK),
        sf::Vector2f(BLOCK, 4 * BLOCK),
        sf::Vector2f(BLOCK, 4 * BLOCK),
        sf::Vector2f(BLOCK, 4 * BLOCK),
        sf::Vector2f(BLOCK, 4 * BLOCK),
        sf::Vector2f(BLOCK, 4 * BLOCK),
        sf::Vector2f(BLOCK, 4 * BLOCK),
        sf::Vector2f(BLOCK, 4 * BLOCK),
        sf::Vector2f(BLOCK, 4 * BLOCK),

        // long blocks in between pilars
        sf::Vector2f(10 * BLOCK, BLOCK),
        sf::Vector2f(10 * BLOCK, BLOCK),
        sf::Vector2f(10 * BLOCK, BLOCK),
        sf::Vector2f(10 * BLOCK, BLOCK),

        // Small blocks in between pilars
        sf::Vector2f(BLOCK, BLOCK),
        sf::Vector2f(BLOCK, BLOCK),
        sf::Vector2f(BLOCK, BLOCK),
        sf::Vector2f(BLOCK, BLOCK),
        sf::Vector2f(BLOCK, BLOCK),
        sf::Vector2f(BLOCK, BLOCK),

        // Center
        sf::Vector2f(2 * BLOCK, 2 * BLOCK),
        sf::Vector2f(6 * BLOCK, BLOCK),
    };

    objPositions = {
        // pilars
        // left side
        std::make_pair(START_LEVEL_X, START_LEVEL_Y),
        std::make_pair(START_LEVEL_X, START_LEVEL_Y - 5 * BLOCK),
        std::make_pair(START_LEVEL_X + 3 * BLOCK, START_LEVEL_Y),
        std::make_pair(START_LEVEL_X + 3 * BLOCK, START_LEVEL_Y - 5 * BLOCK),
        std::make_pair(START_LEVEL_X + 6 * BLOCK, START_LEVEL_Y),
        std::make_pair(START_LEVEL_X + 6 * BLOCK, START_LEVEL_Y - 5 * BLOCK),
        std::make_pair(START_LEVEL_X + 9 * BLOCK, START_LEVEL_Y),
        std::make_pair(START_LEVEL_X + 9 * BLOCK, START_LEVEL_Y - 5 * BLOCK),

        // right side
        std::make_pair(START_LEVEL_X + 14 * BLOCK, START_LEVEL_Y),
        std::make_pair(START_LEVEL_X + 14 * BLOCK, START_LEVEL_Y - 5 * BLOCK),
        std::make_pair(START_LEVEL_X + 17 * BLOCK, START_LEVEL_Y),
        std::make_pair(START_LEVEL_X + 17 * BLOCK, START_LEVEL_Y - 5 * BLOCK),
        std::make_pair(START_LEVEL_X + 20 * BLOCK, START_LEVEL_Y),
        std::make_pair(START_LEVEL_X + 20 * BLOCK, START_LEVEL_Y - 5 * BLOCK),
        std::make_pair(START_LEVEL_X + 23 * BLOCK, START_LEVEL_Y),
        std::make_pair(START_LEVEL_X + 23 * BLOCK, START_LEVEL_Y - 5 * BLOCK),

        // long blocks in between pilars
        std::make_pair(START_LEVEL_X, START_LEVEL_Y - BLOCK),
        std::make_pair(START_LEVEL_X, START_LEVEL_Y - 6 * BLOCK),
        std::make_pair(START_LEVEL_X + 14 * BLOCK, START_LEVEL_Y - BLOCK),
        std::make_pair(START_LEVEL_X + 14 * BLOCK, START_LEVEL_Y - 6 * BLOCK),

        // Small blocks in between pilars
        // left
        std::make_pair(START_LEVEL_X + 1.5 * BLOCK, START_LEVEL_Y - 7 * BLOCK),
        std::make_pair(START_LEVEL_X + 4.5 * BLOCK, START_LEVEL_Y - 7 * BLOCK),
        std::make_pair(START_LEVEL_X + 7.5 * BLOCK, START_LEVEL_Y - 7 * BLOCK),
        // right
        std::make_pair(START_LEVEL_X + 15.5 * BLOCK, START_LEVEL_Y - 7 * BLOCK),
        std::make_pair(START_LEVEL_X + 18.5 * BLOCK, START_LEVEL_Y - 7 * BLOCK),
        std::make_pair(START_LEVEL_X + 21.5 * BLOCK, START_LEVEL_Y - 7 * BLOCK),

        // Center
        std::make_pair(START_LEVEL_X + 11 * BLOCK, START_LEVEL_Y - 9 * BLOCK),
        std::make_pair(START_LEVEL_X + 9 * BLOCK, START_LEVEL_Y - 7 * BLOCK),
    };

    objColors = {
        {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0},
        {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0},
        {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0},
        {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0},
        {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0},
    };

    l->setObjects(objSizes, objPositions, objColors);

    objSizes = {sf::Vector2f(bound_x, BLOCK * 2)};
    objPositions = {std::make_pair(0.0f, bound_y - 2 * BLOCK)};
    objColors = {{120, 110, 100}};

    l->setFloor(objSizes, objPositions, objColors);

    objSizes = {// inside
                sf::Vector2f(BLOCK / 2.0f, BLOCK / 2.0f),
                sf::Vector2f(BLOCK / 2.0f, BLOCK / 2.0f),
                sf::Vector2f(BLOCK / 2.0f, BLOCK / 2.0f),
                sf::Vector2f(BLOCK / 2.0f, BLOCK / 2.0f),
                sf::Vector2f(BLOCK / 2.0f, BLOCK / 2.0f),
                sf::Vector2f(BLOCK / 2.0f, BLOCK / 2.0f),
                sf::Vector2f(2 * BLOCK / 2.0f, 2 * BLOCK / 2.0f)};
    objPositions = {
        // inside
        std::make_pair(START_LEVEL_X + 1.5 * BLOCK, START_LEVEL_Y - 2 * BLOCK),
        std::make_pair(START_LEVEL_X + 4.5 * BLOCK, START_LEVEL_Y - 2 * BLOCK),
        std::make_pair(START_LEVEL_X + 7.5 * BLOCK, START_LEVEL_Y - 2 * BLOCK),
        std::make_pair(START_LEVEL_X + 15.5 * BLOCK, START_LEVEL_Y - 2 * BLOCK),
        std::make_pair(START_LEVEL_X + 18.5 * BLOCK, START_LEVEL_Y - 2 * BLOCK),
        std::make_pair(START_LEVEL_X + 21.5 * BLOCK, START_LEVEL_Y - 2 * BLOCK),
        std::make_pair(START_LEVEL_X + 11 * BLOCK, START_LEVEL_Y + 2 * BLOCK),
    };
    objColors = {{9, 186, 45}, {9, 186, 45}, {9, 186, 45}, {9, 186, 45},
                 {9, 186, 45}, {9, 186, 45}, {9, 186, 45}};

    texturePaths.clear();
    for (int i = 0; i < objSizes.size(); ++i) {
      texturePaths.push_back("./assets/textures/milei/kirchner.png");
    }
    l->setTargets(objSizes, objPositions, objColors, texturePaths);

    break;
  case 2:
    bound_x = 1500;
    bound_y = 900;
    START_LEVEL_X = bound_x - BLOCK * 27;
    START_LEVEL_Y = bound_y - BLOCK * 2;

    l->setStarts(START_LEVEL_X, START_LEVEL_Y);

    l->setBounds(bound_x, bound_y);

    objSizes = {
        // first building
        // bottom
        sf::Vector2f(4 * BLOCK, BLOCK),
        // pilars
        sf::Vector2f(BLOCK, 2 * BLOCK),
        sf::Vector2f(BLOCK, 2 * BLOCK),
        // top big
        sf::Vector2f(4 * BLOCK, BLOCK),
        // top  small
        sf::Vector2f(BLOCK, BLOCK),
        sf::Vector2f(BLOCK, BLOCK),
        sf::Vector2f(BLOCK, BLOCK),

        // second building
        // bottom
        sf::Vector2f(4 * BLOCK, BLOCK),
        // pilars
        sf::Vector2f(BLOCK, 2 * BLOCK),
        sf::Vector2f(BLOCK, 2 * BLOCK),
        // top big
        sf::Vector2f(4 * BLOCK, BLOCK),
        // top  small
        sf::Vector2f(BLOCK, BLOCK),
        sf::Vector2f(BLOCK, BLOCK),
        sf::Vector2f(BLOCK, BLOCK),

        // third building
        // bottom
        sf::Vector2f(4 * BLOCK, BLOCK),
        // pilars
        sf::Vector2f(BLOCK, 2 * BLOCK),
        sf::Vector2f(BLOCK, 2 * BLOCK),
        // top big
        sf::Vector2f(4 * BLOCK, BLOCK),
        // top  small
        sf::Vector2f(BLOCK, BLOCK),
        sf::Vector2f(BLOCK, BLOCK),
        sf::Vector2f(BLOCK, BLOCK),
    };

    objPositions = {
        // first building
        // bottom
        std::make_pair(START_LEVEL_X, START_LEVEL_Y - BLOCK),
        // pillars
        std::make_pair(START_LEVEL_X, START_LEVEL_Y - 3 * BLOCK),
        std::make_pair(START_LEVEL_X + 3 * BLOCK, START_LEVEL_Y - 3 * BLOCK),
        // top big
        std::make_pair(START_LEVEL_X, START_LEVEL_Y - 4 * BLOCK),
        // top small
        std::make_pair(START_LEVEL_X, START_LEVEL_Y - 5 * BLOCK),
        std::make_pair(START_LEVEL_X + 1.5 * BLOCK, START_LEVEL_Y - 5 * BLOCK),
        std::make_pair(START_LEVEL_X + 3 * BLOCK, START_LEVEL_Y - 5 * BLOCK),

        // second building
        // bottom
        std::make_pair(START_LEVEL_X + 6 * BLOCK, START_LEVEL_Y - 3 * BLOCK),
        // pillars
        std::make_pair(START_LEVEL_X + 6 * BLOCK, START_LEVEL_Y - 5 * BLOCK),
        std::make_pair(START_LEVEL_X + 9 * BLOCK, START_LEVEL_Y - 5 * BLOCK),
        // top big
        std::make_pair(START_LEVEL_X + 6 * BLOCK, START_LEVEL_Y - 6 * BLOCK),
        // top small
        std::make_pair(START_LEVEL_X + 6 * BLOCK, START_LEVEL_Y - 7 * BLOCK),
        std::make_pair(START_LEVEL_X + 7.5 * BLOCK, START_LEVEL_Y - 7 * BLOCK),
        std::make_pair(START_LEVEL_X + 9 * BLOCK, START_LEVEL_Y - 7 * BLOCK),

        // third building
        // bottom
        std::make_pair(START_LEVEL_X + 12 * BLOCK, START_LEVEL_Y - 5 * BLOCK),
        // pillars
        std::make_pair(START_LEVEL_X + 12 * BLOCK, START_LEVEL_Y - 7 * BLOCK),
        std::make_pair(START_LEVEL_X + 15 * BLOCK, START_LEVEL_Y - 7 * BLOCK),
        // top big
        std::make_pair(START_LEVEL_X + 12 * BLOCK, START_LEVEL_Y - 8 * BLOCK),
        // top small
        std::make_pair(START_LEVEL_X + 12 * BLOCK, START_LEVEL_Y - 9 * BLOCK),
        std::make_pair(START_LEVEL_X + 13.5 * BLOCK, START_LEVEL_Y - 9 * BLOCK),
        std::make_pair(START_LEVEL_X + 15 * BLOCK, START_LEVEL_Y - 9 * BLOCK),
    };

    objColors = {
        {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0},
        {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0},
        {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0},
        {0, 0, 0}, {0, 0, 0}, {0, 0, 0},
    };

    l->setObjects(objSizes, objPositions, objColors);

    objSizes = {
        sf::Vector2f(bound_x, BLOCK * 2),
        sf::Vector2f(BLOCK * 5, BLOCK * 2),
        sf::Vector2f(BLOCK * 7, BLOCK * 4),
    };
    objPositions = {
        std::make_pair(0.0f, bound_y - 2 * BLOCK),
        std::make_pair(START_LEVEL_X + 5.5 * BLOCK, START_LEVEL_Y - 2 * BLOCK),
        std::make_pair(START_LEVEL_X + 10.5 * BLOCK,
                       START_LEVEL_Y - 4 * BLOCK)};
    objColors = {{120, 110, 100}, {120, 110, 100}, {120, 110, 100}};

    l->setFloor(objSizes, objPositions, objColors);

    objSizes = {// inside
                sf::Vector2f(BLOCK / 2.0f, BLOCK / 2.0f),
                sf::Vector2f(BLOCK / 2.0f, BLOCK / 2.0f),
                sf::Vector2f(BLOCK / 2.0f, BLOCK / 2.0f)};
    objPositions = {
        // inside
        std::make_pair(START_LEVEL_X + 1.5 * BLOCK, START_LEVEL_Y - 2 * BLOCK),
        std::make_pair(START_LEVEL_X + 7.5 * BLOCK, START_LEVEL_Y - 4 * BLOCK),
        std::make_pair(START_LEVEL_X + 13.5 * BLOCK, START_LEVEL_Y - 6 * BLOCK),
    };
    objColors = {{9, 186, 45}, {9, 186, 45}, {9, 186, 45}};

    texturePaths.clear();
    texturePaths.push_back("./assets/textures/milei/fernandez.png");
    texturePaths.push_back("./assets/textures/milei/kirchner.png");
    texturePaths.push_back("./assets/textures/milei/massa.png");
    l->setTargets(objSizes, objPositions, objColors, texturePaths);

    break;
  case 3:
    bound_x = 2000;
    bound_y = 1000;
    START_LEVEL_X = bound_x - BLOCK * 26;
    START_LEVEL_Y = bound_y - BLOCK * 2;

    l->setStarts(START_LEVEL_X, START_LEVEL_Y);

    l->setBounds(bound_x, bound_y);

    objSizes = {
        // bottom
        // base
        sf::Vector2f(3 * BLOCK, BLOCK),
        sf::Vector2f(2.5 * BLOCK, BLOCK),
        sf::Vector2f(4 * BLOCK, BLOCK),
        sf::Vector2f(2.5 * BLOCK, BLOCK),
        sf::Vector2f(3 * BLOCK, BLOCK),
        // pilars
        sf::Vector2f(BLOCK, BLOCK * 6),
        sf::Vector2f(BLOCK, BLOCK * 6),
        sf::Vector2f(BLOCK, BLOCK * 6),
        sf::Vector2f(BLOCK, BLOCK * 6),

        // middle
        // lower
        sf::Vector2f(7 * BLOCK, BLOCK),
        sf::Vector2f(5 * BLOCK, BLOCK),
        sf::Vector2f(7 * BLOCK, BLOCK),
        // upper
        sf::Vector2f(9.5 * BLOCK, BLOCK),
        sf::Vector2f(9.5 * BLOCK, BLOCK),

        // top
        // pilars
        sf::Vector2f(BLOCK, 2 * BLOCK),
        sf::Vector2f(BLOCK, 2 * BLOCK),
        sf::Vector2f(BLOCK, 2 * BLOCK),
        sf::Vector2f(BLOCK, 2 * BLOCK),
        sf::Vector2f(BLOCK, 2 * BLOCK),
        sf::Vector2f(BLOCK, 2 * BLOCK),
        sf::Vector2f(BLOCK, 2 * BLOCK),
        sf::Vector2f(BLOCK, 2 * BLOCK),
        sf::Vector2f(BLOCK, 2 * BLOCK),
        // sideways
        sf::Vector2f(6 * BLOCK, BLOCK),
        sf::Vector2f(7 * BLOCK, BLOCK),
        sf::Vector2f(6 * BLOCK, BLOCK),
    };

    objPositions = {
        // bottom
        // base
        std::make_pair(START_LEVEL_X + BLOCK, START_LEVEL_Y - BLOCK),
        std::make_pair(START_LEVEL_X + 5 * BLOCK, START_LEVEL_Y - BLOCK),
        std::make_pair(START_LEVEL_X + 8.5 * BLOCK, START_LEVEL_Y - BLOCK),
        std::make_pair(START_LEVEL_X + 13.5 * BLOCK, START_LEVEL_Y - BLOCK),
        std::make_pair(START_LEVEL_X + 17 * BLOCK, START_LEVEL_Y - BLOCK),
        // pillars
        std::make_pair(START_LEVEL_X + 4 * BLOCK, START_LEVEL_Y - 6 * BLOCK),
        std::make_pair(START_LEVEL_X + 7.5 * BLOCK, START_LEVEL_Y - 6 * BLOCK),
        std::make_pair(START_LEVEL_X + 12.5 * BLOCK, START_LEVEL_Y - 6 * BLOCK),
        std::make_pair(START_LEVEL_X + 16 * BLOCK, START_LEVEL_Y - 6 * BLOCK),

        // middle
        // lower
        std::make_pair(START_LEVEL_X + 1 * BLOCK, START_LEVEL_Y - 7 * BLOCK),
        std::make_pair(START_LEVEL_X + 8 * BLOCK, START_LEVEL_Y - 7 * BLOCK),
        std::make_pair(START_LEVEL_X + 13 * BLOCK, START_LEVEL_Y - 7 * BLOCK),
        // upper
        std::make_pair(START_LEVEL_X + 1 * BLOCK, START_LEVEL_Y - 8 * BLOCK),
        std::make_pair(START_LEVEL_X + 10.5 * BLOCK, START_LEVEL_Y - 8 * BLOCK),

        // top
        // pillars
        std::make_pair(START_LEVEL_X + 2 * BLOCK, START_LEVEL_Y - 10 * BLOCK),
        std::make_pair(START_LEVEL_X + 4 * BLOCK, START_LEVEL_Y - 10 * BLOCK),
        std::make_pair(START_LEVEL_X + 6 * BLOCK, START_LEVEL_Y - 10 * BLOCK),
        std::make_pair(START_LEVEL_X + 8 * BLOCK, START_LEVEL_Y - 10 * BLOCK),
        std::make_pair(START_LEVEL_X + 10 * BLOCK, START_LEVEL_Y - 10 * BLOCK),
        std::make_pair(START_LEVEL_X + 12 * BLOCK, START_LEVEL_Y - 10 * BLOCK),
        std::make_pair(START_LEVEL_X + 14 * BLOCK, START_LEVEL_Y - 10 * BLOCK),
        std::make_pair(START_LEVEL_X + 16 * BLOCK, START_LEVEL_Y - 10 * BLOCK),
        std::make_pair(START_LEVEL_X + 18 * BLOCK, START_LEVEL_Y - 10 * BLOCK),
        // sideways
        std::make_pair(START_LEVEL_X + 1 * BLOCK, START_LEVEL_Y - 11 * BLOCK),
        std::make_pair(START_LEVEL_X + 7 * BLOCK, START_LEVEL_Y - 11 * BLOCK),
        std::make_pair(START_LEVEL_X + 14 * BLOCK, START_LEVEL_Y - 11 * BLOCK),
    };

    objColors = {
        {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0},
        {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0},
        {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0},
        {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0},
        {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0},
    };

    l->setObjects(objSizes, objPositions, objColors);

    objSizes = {
        sf::Vector2f(bound_x, BLOCK * 2),
        // sides
        sf::Vector2f(BLOCK, BLOCK * 12),
        sf::Vector2f(BLOCK, BLOCK * 12),
        // square
        sf::Vector2f(BLOCK, BLOCK),
        sf::Vector2f(BLOCK, BLOCK),
    };
    objPositions = {
        std::make_pair(0.0f, bound_y - 2 * BLOCK),
        // sides
        std::make_pair(START_LEVEL_X, START_LEVEL_Y - 12 * BLOCK),
        std::make_pair(START_LEVEL_X + 20 * BLOCK, START_LEVEL_Y - 12 * BLOCK),
        // squares
        std::make_pair(START_LEVEL_X + 1 * BLOCK, START_LEVEL_Y - 6 * BLOCK),
        std::make_pair(START_LEVEL_X + 19 * BLOCK, START_LEVEL_Y - 6 * BLOCK),
    };
    objColors = {{120, 110, 100},
                 {120, 110, 100},
                 {120, 110, 100},
                 {120, 110, 100},
                 {120, 110, 100}};

    l->setFloor(objSizes, objPositions, objColors);

    objSizes = {
        sf::Vector2f(BLOCK / 2.0f, BLOCK / 2.0f),
        sf::Vector2f(2 * BLOCK / 2.0f, 2 * BLOCK / 2.0f),
        sf::Vector2f(BLOCK / 2.0f, BLOCK / 2.0f),
    };
    objPositions = {
        std::make_pair(START_LEVEL_X + 2 * BLOCK, START_LEVEL_Y - 2 * BLOCK),
        std::make_pair(START_LEVEL_X + 10 * BLOCK, START_LEVEL_Y - 3 * BLOCK),
        std::make_pair(START_LEVEL_X + 18 * BLOCK, START_LEVEL_Y - 2 * BLOCK),
    };
    objColors = {{9, 186, 45}, {9, 186, 45}, {9, 186, 45}};

    texturePaths.clear();
    for (int i = 0; i < objSizes.size(); ++i) {
      texturePaths.push_back("./assets/textures/milei/kirchner.png");
    }
    l->setTargets(objSizes, objPositions, objColors, texturePaths);

    break;

  case 4:
    // Level for testing the killing of birds
    bound_x = 2000;
    bound_y = 1000;
    START_LEVEL_X = bound_x - BLOCK * 26;
    START_LEVEL_Y = bound_y - BLOCK * 2;

    l->setStarts(START_LEVEL_X, START_LEVEL_Y);

    l->setBounds(bound_x, bound_y);

    objSizes = {};

    objPositions = {};

    objColors = {};

    l->setObjects(objSizes, objPositions, objColors);

    objSizes = {
        sf::Vector2f(bound_x, BLOCK * 2),
        sf::Vector2f(BLOCK, BLOCK * 12),
    };
    objPositions = {
        std::make_pair(0.0f, bound_y - 2 * BLOCK),
        // sides
        std::make_pair(START_LEVEL_X + 20 * BLOCK, START_LEVEL_Y - 12 * BLOCK),
    };
    objColors = {{120, 110, 100}, {120, 110, 100}};

    l->setFloor(objSizes, objPositions, objColors);

    objSizes = {
        sf::Vector2f(BLOCK / 2.0f, BLOCK / 2.0f),
    };
    objPositions = {
        std::make_pair(START_LEVEL_X + 2 * BLOCK, START_LEVEL_Y - 2 * BLOCK),
    };
    objColors = {{9, 186, 45}};

    texturePaths.clear();
    for (int i = 0; i < objSizes.size(); ++i) {
      texturePaths.push_back("./assets/textures/milei/kirchner.png");
    }
    l->setTargets(objSizes, objPositions, objColors, texturePaths);

    break;
  }

  return l;
}

void test_level(int level) {
  sf::RenderWindow ventana(sf::VideoMode(WIDTH, HEIGHT),
                           "Angry Birds Ultimate");
  ventana.setFramerateLimit(60);

  sf::RectangleShape fondo({800.0f, 600.0f});
  fondo.setFillColor({135, 206, 235});

  Level *lev = return_level(0, WIDTH, HEIGHT);

  while (ventana.isOpen()) {
    sf::Event evento;
    while (ventana.pollEvent(evento)) {
      if (evento.type == sf::Event::Closed)
        ventana.close();
    }

    ventana.clear(sf::Color(50, 50, 150)); // Dark blue background
    ventana.draw(fondo);
    lev->render(ventana);
    ventana.display();
  }
}
