#include "Levels.h"
#include "Constants.h"
#include "PhysicsWrapper.h"
#include "helper.h"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/ConvexShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Vector2.hpp>
#include <iostream>
#include <utility>
#include <vector>

int WIDTH = 800;
int HEIGHT = 600;

Level::Level() : m_physics(b2Vec2(0.0f, 9.8f)) {
  // Inicializa la gravedad del nivel
}

Level::~Level() {}

// Factory methods para crear diferentes tipos de shapes SFML
std::unique_ptr<sf::Shape> Level::createSFMLShape(SFMLShapeType type,
                                                  b2Body *body,
                                                  const sf::Color &color) {

  if (type == SFMLShapeType::CIRCLE) {
    // TODO
  } else {
    // Is a polygon
    b2Fixture *fixture = body->GetFixtureList();
    sf::Vector2f pos = metersToPixels(body->GetPosition());
    float angle = body->GetAngle() * 180.f / b2_pi;

    b2PolygonShape *polyShape = (b2PolygonShape *)fixture->GetShape();
    int vertexCount = polyShape->m_count;
    sf::ConvexShape convex;
    convex.setPointCount(vertexCount);
    for (int i = 0; i < vertexCount; i++) {
      sf::Vector2f point = metersToPixels(polyShape->m_vertices[i]);
      convex.setPoint(i, point);
    }
    // Check for misalignments
    // TODO
    convex.setPosition(pos);
    convex.setRotation(angle);
    convex.setFillColor(color);
    convex.setOutlineColor({170, 143, 24});
    convex.setOutlineThickness(LINE_THICKNESS);

    return std::make_unique<sf::ConvexShape>(convex);
  }

  return nullptr;
}

// Factory methods para crear diferentes tipos de shapes bodies
b2Body *Level::createBody(SFMLShapeType type, const sf::Vector2f &size,
                          const std::vector<sf::Vector2f> &positions,
                          const sf::Color &color, bool dynamic) {

  // Physics - crear cuerpo estático
  b2Body *temp = nullptr;

  // Review parameters for incorrect positioning

  switch (type) {
  case SFMLShapeType::CIRCLE: {
    temp = createCircle(positions[0].x, positions[0].y, size.x, dynamic);

    break;
  }
  case SFMLShapeType::RECTANGLE: {
    temp = createBox(positions[0].x + size.x / 2.0f,
                     positions[0].y + size.y / 2.0f, size.x / 2.0f,
                     size.y / 2.0f, dynamic);

    break;
  }
  case SFMLShapeType::TRIANGLE: {
    temp = createTriangle(positions[0].x, positions[0].y, size.x, dynamic);
    break;
  }
  case SFMLShapeType::HEXAGON: {
    temp = createHexagon(positions[0].x, positions[0].y, size.x, dynamic);
    break;
  }
  case SFMLShapeType::PENTAGON: {
    // TODO
    break;
  }
  case SFMLShapeType::CUSTOM: {
    // TODO
    temp = createCustom(positions, dynamic);
    break;
  }
  default: {
    break;
  }
  }
  if (temp != nullptr) {
    temp->SetSleepingAllowed(true);
    temp->SetAwake(false);
  }

  return temp;
}

void Level::setObjects(std::vector<sf::Vector2f> &objectSizes,
                       std::vector<std::vector<sf::Vector2f>> &objectPos,
                       std::vector<sf::Color> &objectColors,
                       const std::vector<SFMLShapeType> &shapeTypes,
                       const std::vector<std::string> &texturePaths) {
  // Assume good behavior, all vectos are of the same size
  if (!texturePaths.empty()) {
    this->object_textures.resize(texturePaths.size());
  }

  for (int i = 0; i < objectSizes.size(); i++) {
    SFMLShapeType shapeType = shapeTypes[i];

    // Physics - crear cuerpo físico apropiado según el tipo
    b2Body *temp = createBody(shapeType, objectSizes[i], objectPos[i],
                              objectColors[i], true);
    bodyLife *object_life = new bodyLife(OBJECT_LIFE, OBJECT_DEFENSE);
    temp->SetUserData(object_life);

    if (!temp)
      continue;

    auto shape = createSFMLShape(shapeType, temp, objectColors[i]);

    if (shape) {
      if (!texturePaths.empty() && i < texturePaths.size()) {
        if (this->object_textures[i].loadFromFile(texturePaths[i])) {
          std::cout << "Loaded texture for object " << i << std::endl;
          shape->setTexture(&this->object_textures[i]);
        }
      }
      m_bodies.push_back(temp);
      objects.push_back(std::move(shape));
    }
  }
}

b2Body *Level::createHexagon(float x, float y, float radius, bool dynamic) {
  b2BodyDef bodyDef;
  if (dynamic) {
    bodyDef.type = b2_dynamicBody;
  }
  bodyDef.position.Set(x / SCALE, y / SCALE);
  b2Body *hexaBody = m_physics.CreateBody(&bodyDef);

  b2PolygonShape hexagonShape;
  b2Vec2 vertices[6];
  float angle = 0.0f;
  for (int i = 0; i < 6; i++) {
    vertices[i].Set((radius / SCALE) * std::cos(angle),
                    (radius / SCALE) * std::sin(angle));
    angle += 60.0f * b2_pi / 180.0f;
  }
  hexagonShape.Set(vertices, 6);

  m_physics.CreatePolygonFixture(hexaBody, &hexagonShape, 1.2f);

  hexaBody->SetSleepingAllowed(true);
  hexaBody->SetAwake(false);

  return hexaBody;
}

b2Body *Level::createTriangle(float x, float y, float size, bool dynamic) {
  b2BodyDef bodyDef;
  if (dynamic) {
    bodyDef.type = b2_dynamicBody;
  }
  bodyDef.position.Set(x / SCALE, y / SCALE);
  b2Body *triangleBody = m_physics.CreateBody(&bodyDef);
  b2PolygonShape triangleShape;
  b2Vec2 vertices[3];
  vertices[0].Set(0.0f, -size / SCALE);
  vertices[1].Set(size / SCALE, size / SCALE);
  vertices[2].Set(-size / SCALE, size / SCALE);
  triangleShape.Set(vertices, 3);
  m_physics.CreatePolygonFixture(triangleBody, &triangleShape, 1.5f);

  triangleBody->SetSleepingAllowed(true);
  triangleBody->SetAwake(false);
  return triangleBody;
}

b2Body *Level::createCustom(const std::vector<sf::Vector2f> &points,
                            bool dynamic) {
  // 1. Validate input points
  if (points.size() < 3) { // A polygon needs at least 3 vertices
    std::cerr << "Error: Custom polygon needs at least 3 points. Provided: "
              << points.size() << ".\n";
    return nullptr;
  }
  if (points.size() > b2_maxPolygonVertices) {
    std::cerr << "Error: Custom polygon has too many vertices ("
              << points.size() << "). Max allowed is " << b2_maxPolygonVertices
              << ".\n";
    return nullptr;
  }

  b2BodyDef bodyDef;
  if (dynamic) {
    bodyDef.type = b2_dynamicBody;
  }

  // 2. Calculate the centroid of the polygon (in pixel coordinates)
  sf::Vector2f centroidPixels(0.0f, 0.0f);
  for (const auto &p : points) {
    centroidPixels.x += p.x;
    centroidPixels.y += p.y;
  }
  centroidPixels.x /= static_cast<float>(points.size());
  centroidPixels.y /= static_cast<float>(points.size());

  // Set body position to the calculated centroid (converted to meters)
  bodyDef.position = pixelsToMeters(centroidPixels);

  b2Body *customBody = m_physics.CreateBody(&bodyDef);

  // 3. Convert SFML points to Box2D vertices (relative to the centroid)
  b2Vec2 vertices[b2_maxPolygonVertices];
  for (size_t i = 0; i < points.size(); ++i) {
    // Convert point from pixels to meters
    b2Vec2 pointInMeters = pixelsToMeters(points[i]);
    // Make it relative to the body's origin (which is the centroid)
    vertices[i] = pointInMeters - bodyDef.position;
  }

  // 4. Create the polygon shape
  b2PolygonShape polygonShape;
  // Box2D automatically computes the convex hull and ensures winding order
  polygonShape.Set(vertices, static_cast<int32>(points.size()));

  // 5. Create the fixture
  m_physics.CreatePolygonFixture(customBody, &polygonShape, 1.5f);

  customBody->SetSleepingAllowed(true);
  customBody->SetAwake(false);

  return customBody;
}

b2Body *Level::createBox(float x, float y, float halfWidth, float halfHeight,
                         bool dynamic) {
  b2BodyDef bodyDef;
  if (dynamic) {
    bodyDef.type = b2_dynamicBody;
  }
  bodyDef.position.Set(x / SCALE, y / SCALE);
  b2Body *boxBody = m_physics.CreateBody(&bodyDef);

  m_physics.CreateBoxFixture(boxBody, halfWidth / SCALE, halfHeight / SCALE,
                             1.0f);

  boxBody->SetSleepingAllowed(true);
  boxBody->SetAwake(false);

  return boxBody;
}

b2Body *Level::createCircle(float x, float y, float radius, bool dynamic) {
  b2BodyDef bodyDef;
  if (dynamic) {
    bodyDef.type = b2_dynamicBody;
  }
  bodyDef.position.Set(x / SCALE, y / SCALE);
  b2Body *circleBody = m_physics.CreateBody(&bodyDef);

  b2CircleShape circleShape;
  circleShape.m_radius = radius / SCALE;
  m_physics.CreateCircleFixture(circleBody, &circleShape, 1.0f);

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

void Level::setTargets(std::vector<sf::Vector2f> &objectSizes,
                       std::vector<std::vector<sf::Vector2f>> &objectPos,
                       std::vector<sf::Color> &objectColors,
                       const std::vector<std::string> &texturePaths) {

  if (!texturePaths.empty()) {
    this->target_textures.resize(texturePaths.size());
  }
  for (int i = 0; i < objectSizes.size(); i++) {
    // Physics
    b2Body *temp_target = nullptr;
    b2BodyDef targetDef;
    targetDef.type = b2_dynamicBody;
    targetDef.position.Set((objectPos[i][0].x + objectSizes[i].x) / SCALE,
                           (objectPos[i][0].y + objectSizes[i].y) / SCALE);
    temp_target = m_physics.CreateBody(&targetDef);

    b2CircleShape targetShape;
    targetShape.m_radius = objectSizes[i].x / SCALE;
    m_physics.CreateCircleFixture(temp_target, &targetShape, 0.5f);

    temp_target->SetSleepingAllowed(true);
    temp_target->SetAwake(false);

    m_targets.push_back(temp_target);

    bodyLife *object_life = new bodyLife(TARGET_LIFE, TARGET_DEFENSE);
    temp_target->SetUserData(object_life);

    // SFML
    b2Fixture *fixture = temp_target->GetFixtureList();
    b2Shape::Type shapeType = fixture->GetType();
    sf::Vector2f pos = metersToPixels(temp_target->GetPosition());
    float angle = temp_target->GetAngle() * 180.f / b2_pi;

    sf::CircleShape circle(fixture->GetShape()->m_radius * SCALE);
    if (!texturePaths.empty() && i < texturePaths.size()) {
      if (this->target_textures[i].loadFromFile(texturePaths[i])) {
        circle.setTexture(&this->target_textures[i]);
      } else {
        circle.setFillColor(objectColors[i]);
      }
    } else {
      circle.setFillColor(objectColors[i]);
    }
    circle.setOrigin(circle.getRadius(), circle.getRadius());
    circle.setPosition(pos);
    circle.setRotation(angle);

    targets.push_back(circle);
  }
}

void Level::setFloor(std::vector<sf::Vector2f> &objectSizes,
                     std::vector<std::vector<sf::Vector2f>> &objectPos,
                     std::vector<sf::Color> &objectColors,
                     const std::vector<SFMLShapeType> &shapeTypes,
                     const std::vector<std::string> &texturePaths) {
  // Assuming good behavior and the 4 vectors are equal
  if (!texturePaths.empty()) {
    this->floor_textures.resize(texturePaths.size());
  }

  for (int i = 0; i < objectSizes.size(); i++) {
    // Determinar tipo de shape (por defecto RECTANGLE para el suelo)
    SFMLShapeType shapeType = shapeTypes[i];

    b2Body *temp_static = createBody(shapeType, objectSizes[i], objectPos[i],
                                     objectColors[i], false);

    if (!temp_static)
      continue;

    // SFML - crear shape apropiado
    auto shape = createSFMLShape(shapeType, temp_static, objectColors[i]);
    if (shape) {
      if (!texturePaths.empty() && i < texturePaths.size()) {
        if (this->floor_textures[i].loadFromFile(texturePaths[i])) {
          shape->setTexture(&this->floor_textures[i]);
          shape->setTextureRect(sf::IntRect(0, 0, objectSizes[i].x, objectSizes[i].y));
        }
      }
      m_static.push_back(temp_static);
      floor.push_back(std::move(shape));
    }
  }
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
  for (auto &floorShape : this->floor) {
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
    b2Body *body = m_targets[i];
    sf::Vector2f pos = metersToPixels(body->GetPosition());
    if (pos.x > x_bound + 100 || pos.x < -100 || pos.y > y_bound + 100) {
      idx_remove.push_back(i);
      continue;
    }

    // Update position and rotation from physics body
    b2Fixture *fixture = body->GetFixtureList();

    while (fixture) {
      b2Shape::Type shapeType = fixture->GetType();
      pos = metersToPixels(body->GetPosition());
      float angle = body->GetAngle() * 180.f / b2_pi;

      b2PolygonShape *poly = (b2PolygonShape *)fixture->GetShape();
      // Asumimos que es una caja creada con SetAsBox
      b2Vec2 halfSize = poly->m_vertices[2];

      sf::CircleShape circle = targets[i];
      circle.setPosition(pos);
      circle.setRotation(angle);
      if (i < this->target_textures.size()) {
        circle.setTexture(&this->target_textures[i]);
      }
      ventana.draw(circle);

      fixture = fixture->GetNext();
    }
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
  std::vector<std::vector<sf::Vector2f>> objPositions;
  std::vector<sf::Color> objColors;
  std::vector<SFMLShapeType> shapeTypes;
  Level *l = new Level();
  l->setNLevel(level);
  float START_LEVEL_X;
  float START_LEVEL_Y;
  int bound_x;
  int bound_y;
  std::vector<std::string> texturePaths;
  std::vector<std::string> objTexturePaths;
  std::vector<std::string> floorTexturePaths;
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
        {sf::Vector2f(START_LEVEL_X, START_LEVEL_Y - 2 * BLOCK - OFFSET_ABOVE)},
        {sf::Vector2f(START_LEVEL_X, START_LEVEL_Y - OFFSET_ABOVE)},
        {sf::Vector2f(START_LEVEL_X + 6 * BLOCK, START_LEVEL_Y - OFFSET_ABOVE)},
        {sf::Vector2f(START_LEVEL_X + 11 * BLOCK,
                      START_LEVEL_Y - 2 * BLOCK - OFFSET_ABOVE)},
        {sf::Vector2f(START_LEVEL_X + 3 * BLOCK,
                      START_LEVEL_Y - 7 * BLOCK - OFFSET_ABOVE)},
        {sf::Vector2f(START_LEVEL_X + 8 * BLOCK,
                      START_LEVEL_Y - 7 * BLOCK - OFFSET_ABOVE)},
        {sf::Vector2f(START_LEVEL_X + 3 * BLOCK,
                      START_LEVEL_Y - 8 * BLOCK - OFFSET_ABOVE)},
        {sf::Vector2f(START_LEVEL_X + 4 * BLOCK,
                      START_LEVEL_Y - 15 * BLOCK - OFFSET_ABOVE)},
        {sf::Vector2f(START_LEVEL_X + 7 * BLOCK,
                      START_LEVEL_Y - 15 * BLOCK - OFFSET_ABOVE)},
        {sf::Vector2f(START_LEVEL_X + 4 * BLOCK,
                      START_LEVEL_Y - 16 * BLOCK - OFFSET_ABOVE)},
    };

    objColors = {
        {150, 50, 200}, {150, 50, 200}, {150, 50, 200}, {150, 50, 200},
        {150, 50, 200}, {150, 50, 200}, {150, 50, 200}, {200, 100, 50},
        {200, 100, 50}, {150, 50, 200},
    };

    // Add shape types - some rectangles, some pentagons
    shapeTypes = {SFMLShapeType::RECTANGLE, SFMLShapeType::RECTANGLE,
                  SFMLShapeType::RECTANGLE, SFMLShapeType::RECTANGLE,
                  SFMLShapeType::RECTANGLE, SFMLShapeType::RECTANGLE,
                  SFMLShapeType::RECTANGLE, SFMLShapeType::RECTANGLE,
                  SFMLShapeType::RECTANGLE, SFMLShapeType::RECTANGLE};

    for (int i = 0; i < objSizes.size(); ++i) {
      objTexturePaths.push_back("./assets/textures/material/wood.png");
    }

    l->setObjects(objSizes, objPositions, objColors, shapeTypes,
                  objTexturePaths);

    objSizes = {sf::Vector2f(bound_x, BLOCK * 2)};
    objPositions = {{sf::Vector2f(0.0f, bound_y - 2 * BLOCK)}};
    objColors = {{120, 110, 100}};
    shapeTypes = {SFMLShapeType::RECTANGLE};
    floorTexturePaths.push_back("./assets/textures/material/asphalt.png");

    l->setFloor(objSizes, objPositions, objColors, shapeTypes, floorTexturePaths);

    // We use the same vector for simplicity, It holds the radio
    objSizes = {sf::Vector2f(BLOCK / 2.0f, BLOCK / 2.0f)};
    objPositions = {
        {sf::Vector2f(START_LEVEL_X + 5.5 * BLOCK, START_LEVEL_Y - BLOCK)}};
    objColors = {{9, 186, 45}};
    texturePaths.push_back("./assets/textures/multi/peru.png");

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
        sf::Vector2f(3 * BLOCK, 2 * BLOCK),
        sf::Vector2f(6 * BLOCK, BLOCK),
    };

    objPositions = {
        // pilars
        // left side
        {sf::Vector2f(START_LEVEL_X, START_LEVEL_Y)},
        {sf::Vector2f(START_LEVEL_X, START_LEVEL_Y - 5 * BLOCK)},
        {sf::Vector2f(START_LEVEL_X + 3 * BLOCK, START_LEVEL_Y)},
        {sf::Vector2f(START_LEVEL_X + 3 * BLOCK, START_LEVEL_Y - 5 * BLOCK)},
        {sf::Vector2f(START_LEVEL_X + 6 * BLOCK, START_LEVEL_Y)},
        {sf::Vector2f(START_LEVEL_X + 6 * BLOCK, START_LEVEL_Y - 5 * BLOCK)},
        {sf::Vector2f(START_LEVEL_X + 9 * BLOCK, START_LEVEL_Y)},
        {sf::Vector2f(START_LEVEL_X + 9 * BLOCK, START_LEVEL_Y - 5 * BLOCK)},

        // right side
        {sf::Vector2f(START_LEVEL_X + 14 * BLOCK, START_LEVEL_Y)},
        {sf::Vector2f(START_LEVEL_X + 14 * BLOCK, START_LEVEL_Y - 5 * BLOCK)},
        {sf::Vector2f(START_LEVEL_X + 17 * BLOCK, START_LEVEL_Y)},
        {sf::Vector2f(START_LEVEL_X + 17 * BLOCK, START_LEVEL_Y - 5 * BLOCK)},
        {sf::Vector2f(START_LEVEL_X + 20 * BLOCK, START_LEVEL_Y)},
        {sf::Vector2f(START_LEVEL_X + 20 * BLOCK, START_LEVEL_Y - 5 * BLOCK)},
        {sf::Vector2f(START_LEVEL_X + 23 * BLOCK, START_LEVEL_Y)},
        {sf::Vector2f(START_LEVEL_X + 23 * BLOCK, START_LEVEL_Y - 5 * BLOCK)},

        // long blocks in between pilars
        {sf::Vector2f(START_LEVEL_X, START_LEVEL_Y - BLOCK)},
        {sf::Vector2f(START_LEVEL_X, START_LEVEL_Y - 6 * BLOCK)},
        {sf::Vector2f(START_LEVEL_X + 14 * BLOCK, START_LEVEL_Y - BLOCK)},
        {sf::Vector2f(START_LEVEL_X + 14 * BLOCK, START_LEVEL_Y - 6 * BLOCK)},

        // Small blocks in between pilars
        // left
        {sf::Vector2f(START_LEVEL_X + 2 * BLOCK, START_LEVEL_Y - 7 * BLOCK)},
        {sf::Vector2f(START_LEVEL_X + 5 * BLOCK, START_LEVEL_Y - 7 * BLOCK)},
        {sf::Vector2f(START_LEVEL_X + 8 * BLOCK, START_LEVEL_Y - 7 * BLOCK)},
        // right
        {sf::Vector2f(START_LEVEL_X + 16 * BLOCK, START_LEVEL_Y - 7 * BLOCK)},
        {sf::Vector2f(START_LEVEL_X + 19 * BLOCK, START_LEVEL_Y - 7 * BLOCK)},
        {sf::Vector2f(START_LEVEL_X + 22 * BLOCK, START_LEVEL_Y - 7 * BLOCK)},

        // Center
        {sf::Vector2f(START_LEVEL_X + 12 * BLOCK, START_LEVEL_Y - 10 * BLOCK)},
        {sf::Vector2f(START_LEVEL_X + 9 * BLOCK, START_LEVEL_Y - 7 * BLOCK)},
    };

    objColors = {{150, 50, 200}, {150, 50, 200}, {150, 50, 200}, {150, 50, 200},
                 {150, 50, 200}, {150, 50, 200}, {150, 50, 200}, {150, 50, 200},
                 {150, 50, 200}, {150, 50, 200}, {150, 50, 200}, {150, 50, 200},
                 {150, 50, 200}, {150, 50, 200}, {150, 50, 200}, {150, 50, 200},
                 {150, 50, 200}, {150, 50, 200}, {150, 50, 200}, {150, 50, 200},
                 {150, 50, 200}, {150, 50, 200}, {150, 50, 200}, {150, 50, 200},
                 {150, 50, 200}, {150, 50, 200}, {150, 50, 200}, {150, 50, 200},
                 {150, 50, 200}, {150, 50, 200}};

    shapeTypes = {SFMLShapeType::RECTANGLE, SFMLShapeType::RECTANGLE,
                  SFMLShapeType::RECTANGLE, SFMLShapeType::RECTANGLE,
                  SFMLShapeType::RECTANGLE, SFMLShapeType::RECTANGLE,
                  SFMLShapeType::RECTANGLE, SFMLShapeType::RECTANGLE,
                  SFMLShapeType::RECTANGLE, SFMLShapeType::RECTANGLE,
                  SFMLShapeType::RECTANGLE, SFMLShapeType::RECTANGLE,
                  SFMLShapeType::RECTANGLE, SFMLShapeType::RECTANGLE,
                  SFMLShapeType::RECTANGLE, SFMLShapeType::RECTANGLE,
                  SFMLShapeType::RECTANGLE, SFMLShapeType::RECTANGLE,
                  SFMLShapeType::RECTANGLE, SFMLShapeType::RECTANGLE,
                  SFMLShapeType::TRIANGLE,  SFMLShapeType::TRIANGLE,
                  SFMLShapeType::TRIANGLE,  SFMLShapeType::TRIANGLE,
                  SFMLShapeType::TRIANGLE,  SFMLShapeType::TRIANGLE,
                  SFMLShapeType::TRIANGLE,  SFMLShapeType::RECTANGLE};

    objTexturePaths.clear();
    for (int i = 0; i < objSizes.size(); ++i) {
      objTexturePaths.push_back("./assets/textures/material/wood.png");
    }
    l->setObjects(objSizes, objPositions, objColors, shapeTypes,
                  objTexturePaths);

    objSizes = {sf::Vector2f(bound_x, BLOCK * 2)};
    objPositions = {{sf::Vector2f(0.0f, bound_y - 2 * BLOCK)}};
    objColors = {{120, 110, 100}};
    shapeTypes = {SFMLShapeType::RECTANGLE};

    floorTexturePaths.push_back("./assets/textures/material/asphalt.png");
    l->setFloor(objSizes, objPositions, objColors, shapeTypes, floorTexturePaths);

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
        {sf::Vector2f(START_LEVEL_X + 1.5 * BLOCK, START_LEVEL_Y - 2 * BLOCK)},
        {sf::Vector2f(START_LEVEL_X + 4.5 * BLOCK, START_LEVEL_Y - 2 * BLOCK)},
        {sf::Vector2f(START_LEVEL_X + 7.5 * BLOCK, START_LEVEL_Y - 2 * BLOCK)},
        {sf::Vector2f(START_LEVEL_X + 15.5 * BLOCK, START_LEVEL_Y - 2 * BLOCK)},
        {sf::Vector2f(START_LEVEL_X + 18.5 * BLOCK, START_LEVEL_Y - 2 * BLOCK)},
        {sf::Vector2f(START_LEVEL_X + 21.5 * BLOCK, START_LEVEL_Y - 2 * BLOCK)},
        {sf::Vector2f(START_LEVEL_X + 11 * BLOCK, START_LEVEL_Y + 2 * BLOCK)},
    };
    objColors = {{9, 186, 45}, {9, 186, 45}, {9, 186, 45}, {9, 186, 45},
                 {9, 186, 45}, {9, 186, 45}, {9, 186, 45}};

    texturePaths.clear();
    for (int i = 0; i < objSizes.size(); ++i) {
      texturePaths.push_back("./assets/textures/fujimori/olivera.png");
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

        // Fourth building
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
        {sf::Vector2f(START_LEVEL_X, START_LEVEL_Y - BLOCK)},
        // pillars
        {sf::Vector2f(START_LEVEL_X, START_LEVEL_Y - 3 * BLOCK)},
        {sf::Vector2f(START_LEVEL_X + 3 * BLOCK, START_LEVEL_Y - 3 * BLOCK)},
        // top big
        {sf::Vector2f(START_LEVEL_X, START_LEVEL_Y - 4 * BLOCK)},
        // top small
        {sf::Vector2f(START_LEVEL_X, START_LEVEL_Y - 5 * BLOCK)},
        {sf::Vector2f(START_LEVEL_X + 1.5 * BLOCK, START_LEVEL_Y - 5 * BLOCK)},
        {sf::Vector2f(START_LEVEL_X + 3 * BLOCK, START_LEVEL_Y - 5 * BLOCK)},

        // second building
        // bottom
        {sf::Vector2f(START_LEVEL_X + 6 * BLOCK, START_LEVEL_Y - 6 * BLOCK)},
        // pillars
        {sf::Vector2f(START_LEVEL_X + 6 * BLOCK, START_LEVEL_Y - 8 * BLOCK)},
        {sf::Vector2f(START_LEVEL_X + 9 * BLOCK, START_LEVEL_Y - 8 * BLOCK)},
        // top big
        {sf::Vector2f(START_LEVEL_X + 6 * BLOCK, START_LEVEL_Y - 9 * BLOCK)},
        // top small
        {sf::Vector2f(START_LEVEL_X + 6 * BLOCK, START_LEVEL_Y - 10 * BLOCK)},
        {sf::Vector2f(START_LEVEL_X + 7.5 * BLOCK, START_LEVEL_Y - 10 * BLOCK)},
        {sf::Vector2f(START_LEVEL_X + 9 * BLOCK, START_LEVEL_Y - 10 * BLOCK)},

        // third building
        // bottom
        {sf::Vector2f(START_LEVEL_X + 11 * BLOCK, START_LEVEL_Y - 11 * BLOCK)},
        // pillars
        {sf::Vector2f(START_LEVEL_X + 11 * BLOCK, START_LEVEL_Y - 13 * BLOCK)},
        {sf::Vector2f(START_LEVEL_X + 14 * BLOCK, START_LEVEL_Y - 13 * BLOCK)},
        // top big
        {sf::Vector2f(START_LEVEL_X + 11 * BLOCK, START_LEVEL_Y - 14 * BLOCK)},
        // top small
        {sf::Vector2f(START_LEVEL_X + 11 * BLOCK, START_LEVEL_Y - 15 * BLOCK)},
        {sf::Vector2f(START_LEVEL_X + 12.5 * BLOCK,
                      START_LEVEL_Y - 15 * BLOCK)},
        {sf::Vector2f(START_LEVEL_X + 14 * BLOCK, START_LEVEL_Y - 15 * BLOCK)},

        // fourth building
        // bottom
        {sf::Vector2f(START_LEVEL_X + 16 * BLOCK, START_LEVEL_Y - 16 * BLOCK)},
        // pillars
        {sf::Vector2f(START_LEVEL_X + 16 * BLOCK, START_LEVEL_Y - 18 * BLOCK)},
        {sf::Vector2f(START_LEVEL_X + 19 * BLOCK, START_LEVEL_Y - 18 * BLOCK)},
        // top big
        {sf::Vector2f(START_LEVEL_X + 16 * BLOCK, START_LEVEL_Y - 19 * BLOCK)},
        // top small
        {sf::Vector2f(START_LEVEL_X + 16 * BLOCK, START_LEVEL_Y - 20 * BLOCK)},
        {sf::Vector2f(START_LEVEL_X + 17.5 * BLOCK,
                      START_LEVEL_Y - 20 * BLOCK)},
        {sf::Vector2f(START_LEVEL_X + 19 * BLOCK, START_LEVEL_Y - 20 * BLOCK)},
    };

    objColors = {
        {150, 50, 200}, {150, 50, 200}, {150, 50, 200}, {150, 50, 200},
        {150, 50, 200}, {150, 50, 200}, {150, 50, 200}, {150, 50, 200},
        {150, 50, 200}, {150, 50, 200}, {150, 50, 200}, {150, 50, 200},
        {150, 50, 200}, {150, 50, 200}, {150, 50, 200}, {150, 50, 200},
        {150, 50, 200}, {150, 50, 200}, {150, 50, 200}, {150, 50, 200},
        {150, 50, 200}, {150, 50, 200}, {150, 50, 200}, {150, 50, 200},
        {150, 50, 200}, {150, 50, 200}, {150, 50, 200}, {150, 50, 200},
        {150, 50, 200}, {150, 50, 200}, {150, 50, 200},
    };

    shapeTypes = {SFMLShapeType::RECTANGLE, SFMLShapeType::RECTANGLE,
                  SFMLShapeType::RECTANGLE, SFMLShapeType::RECTANGLE,
                  SFMLShapeType::RECTANGLE, SFMLShapeType::RECTANGLE,
                  SFMLShapeType::RECTANGLE, SFMLShapeType::RECTANGLE,
                  SFMLShapeType::RECTANGLE, SFMLShapeType::RECTANGLE,
                  SFMLShapeType::RECTANGLE, SFMLShapeType::RECTANGLE,
                  SFMLShapeType::RECTANGLE, SFMLShapeType::RECTANGLE,
                  SFMLShapeType::RECTANGLE, SFMLShapeType::RECTANGLE,
                  SFMLShapeType::RECTANGLE, SFMLShapeType::RECTANGLE,
                  SFMLShapeType::RECTANGLE, SFMLShapeType::RECTANGLE,
                  SFMLShapeType::RECTANGLE, SFMLShapeType::RECTANGLE,
                  SFMLShapeType::RECTANGLE, SFMLShapeType::RECTANGLE,
                  SFMLShapeType::RECTANGLE, SFMLShapeType::RECTANGLE,
                  SFMLShapeType::RECTANGLE, SFMLShapeType::RECTANGLE,
                  SFMLShapeType::RECTANGLE, SFMLShapeType::RECTANGLE,
                  SFMLShapeType::RECTANGLE, SFMLShapeType::RECTANGLE,
                  SFMLShapeType::RECTANGLE, SFMLShapeType::RECTANGLE,
                  SFMLShapeType::RECTANGLE};

    objTexturePaths.clear();
    for (int i = 0; i < objSizes.size(); ++i) {
      objTexturePaths.push_back("./assets/textures/material/wood.png");
    }
    l->setObjects(objSizes, objPositions, objColors, shapeTypes,
                  objTexturePaths);

    objSizes = {
        sf::Vector2f(bound_x, BLOCK * 2),
        sf::Vector2f(BLOCK * 5, BLOCK * 5),
        sf::Vector2f(BLOCK * 5, BLOCK * 10),
        sf::Vector2f(BLOCK * 5, BLOCK * 15),
        sf::Vector2f(BLOCK * 5, BLOCK * 15),
    };
    objPositions = {
        {sf::Vector2f(0.0f, bound_y - 2 * BLOCK)},
        {sf::Vector2f(START_LEVEL_X + 5.5 * BLOCK, START_LEVEL_Y - 5 * BLOCK)},
        {sf::Vector2f(START_LEVEL_X + 10.5 * BLOCK,
                      START_LEVEL_Y - 10 * BLOCK)},
        {sf::Vector2f(START_LEVEL_X + 15.5 * BLOCK,
                      START_LEVEL_Y - 15 * BLOCK)},
        {
            sf::Vector2f(START_LEVEL_X + 20.5 * BLOCK,
                         START_LEVEL_Y - 15 * BLOCK),
            sf::Vector2f(bound_x, 0),
            sf::Vector2f(bound_x, START_LEVEL_Y),
            sf::Vector2f(START_LEVEL_X + 20.5 * BLOCK, START_LEVEL_Y),
        },
    };
    objColors = {{120, 110, 100},
                 {120, 110, 100},
                 {120, 110, 100},
                 {120, 110, 100},
                 {120, 110, 100}};

    shapeTypes = {SFMLShapeType::RECTANGLE, SFMLShapeType::RECTANGLE,
                  SFMLShapeType::RECTANGLE, SFMLShapeType::RECTANGLE,
                  SFMLShapeType::CUSTOM};

    for (int i = 0; i < objSizes.size(); ++i) {
        floorTexturePaths.push_back("./assets/textures/material/asphalt.png");
    }

    l->setFloor(objSizes, objPositions, objColors, shapeTypes, floorTexturePaths);

    objSizes = {
        // inside
        sf::Vector2f(BLOCK / 2.0f, BLOCK / 2.0f),
        sf::Vector2f(BLOCK / 2.0f, BLOCK / 2.0f),
        sf::Vector2f(BLOCK / 2.0f, BLOCK / 2.0f),
        sf::Vector2f(BLOCK / 2.0f, BLOCK / 2.0f),
    };
    objPositions = {
        // inside
        {sf::Vector2f(START_LEVEL_X + 1.5 * BLOCK, START_LEVEL_Y - 2 * BLOCK)},
        {sf::Vector2f(START_LEVEL_X + 7.5 * BLOCK, START_LEVEL_Y - 7 * BLOCK)},
        {sf::Vector2f(START_LEVEL_X + 12.5 * BLOCK,
                      START_LEVEL_Y - 12 * BLOCK)},
        {sf::Vector2f(START_LEVEL_X + 17.5 * BLOCK,
                      START_LEVEL_Y - 17 * BLOCK)}};
    objColors = {{9, 186, 45}, {9, 186, 45}, {9, 186, 45}, {9, 186, 45}};

    texturePaths.clear();
    texturePaths.push_back("./assets/textures/milei/fernandez.png");
    texturePaths.push_back("./assets/textures/milei/kirchner.png");
    texturePaths.push_back("./assets/textures/milei/massa.png");
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
        sf::Vector2f(2.5 * BLOCK, BLOCK),
        sf::Vector2f(4 * BLOCK, BLOCK),
        sf::Vector2f(2.5 * BLOCK, BLOCK),
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
        // Hexagons
        sf::Vector2f(2 * BLOCK, BLOCK),
        sf::Vector2f(2 * BLOCK, BLOCK),
        sf::Vector2f(2 * BLOCK, BLOCK),
    };

    objPositions = {
        // bottom
        // base
        {sf::Vector2f(START_LEVEL_X + 5 * BLOCK, START_LEVEL_Y - BLOCK)},
        {sf::Vector2f(START_LEVEL_X + 8.5 * BLOCK, START_LEVEL_Y - BLOCK)},
        {sf::Vector2f(START_LEVEL_X + 13.5 * BLOCK, START_LEVEL_Y - BLOCK)},
        // pillars
        {sf::Vector2f(START_LEVEL_X + 4 * BLOCK, START_LEVEL_Y - 6 * BLOCK)},
        {sf::Vector2f(START_LEVEL_X + 7.5 * BLOCK, START_LEVEL_Y - 6 * BLOCK)},
        {sf::Vector2f(START_LEVEL_X + 12.5 * BLOCK, START_LEVEL_Y - 6 * BLOCK)},
        {sf::Vector2f(START_LEVEL_X + 16 * BLOCK, START_LEVEL_Y - 6 * BLOCK)},

        // middle
        // lower
        {sf::Vector2f(START_LEVEL_X + 1 * BLOCK, START_LEVEL_Y - 7 * BLOCK)},
        {sf::Vector2f(START_LEVEL_X + 8 * BLOCK, START_LEVEL_Y - 7 * BLOCK)},
        {sf::Vector2f(START_LEVEL_X + 13 * BLOCK, START_LEVEL_Y - 7 * BLOCK)},
        // upper
        {sf::Vector2f(START_LEVEL_X + 1 * BLOCK, START_LEVEL_Y - 8 * BLOCK)},
        {sf::Vector2f(START_LEVEL_X + 10.5 * BLOCK, START_LEVEL_Y - 8 * BLOCK)},

        // top
        // pillars
        {sf::Vector2f(START_LEVEL_X + 2 * BLOCK, START_LEVEL_Y - 10 * BLOCK)},
        {sf::Vector2f(START_LEVEL_X + 4 * BLOCK, START_LEVEL_Y - 10 * BLOCK)},
        {sf::Vector2f(START_LEVEL_X + 6 * BLOCK, START_LEVEL_Y - 10 * BLOCK)},
        {sf::Vector2f(START_LEVEL_X + 8 * BLOCK, START_LEVEL_Y - 10 * BLOCK)},
        {sf::Vector2f(START_LEVEL_X + 10 * BLOCK, START_LEVEL_Y - 10 * BLOCK)},
        {sf::Vector2f(START_LEVEL_X + 12 * BLOCK, START_LEVEL_Y - 10 * BLOCK)},
        {sf::Vector2f(START_LEVEL_X + 14 * BLOCK, START_LEVEL_Y - 10 * BLOCK)},
        {sf::Vector2f(START_LEVEL_X + 16 * BLOCK, START_LEVEL_Y - 10 * BLOCK)},
        {sf::Vector2f(START_LEVEL_X + 18 * BLOCK, START_LEVEL_Y - 10 * BLOCK)},
        // sideways
        {sf::Vector2f(START_LEVEL_X + 1 * BLOCK, START_LEVEL_Y - 11 * BLOCK)},
        {sf::Vector2f(START_LEVEL_X + 7 * BLOCK, START_LEVEL_Y - 11 * BLOCK)},
        {sf::Vector2f(START_LEVEL_X + 14 * BLOCK, START_LEVEL_Y - 11 * BLOCK)},
        // hexagons
        // Calculate positions precisely is a little anoying
        {sf::Vector2f(START_LEVEL_X + 4 * BLOCK,
                      START_LEVEL_Y - 13 * BLOCK + 7)},
        {sf::Vector2f(START_LEVEL_X + 10.5 * BLOCK,
                      START_LEVEL_Y - 13 * BLOCK + 7)},
        {sf::Vector2f(START_LEVEL_X + 17 * BLOCK,
                      START_LEVEL_Y - 13 * BLOCK + 7)},
    };

    objColors = {
        {230, 230, 230}, {230, 230, 230}, {230, 230, 230}, {230, 230, 230},
        {230, 230, 230}, {230, 230, 230}, {230, 230, 230}, {230, 230, 230},
        {230, 230, 230}, {230, 230, 230}, {230, 230, 230}, {230, 230, 230},
        {230, 230, 230}, {230, 230, 230}, {230, 230, 230}, {230, 230, 230},
        {230, 230, 230}, {230, 230, 230}, {230, 230, 230}, {230, 230, 230},
        {230, 230, 230}, {230, 230, 230}, {230, 230, 230}, {230, 230, 230},
        {230, 230, 230}, {230, 230, 230}, {230, 230, 230}, {230, 230, 230},
        {230, 230, 230}, {230, 230, 230}, {230, 230, 230},
    };

    shapeTypes = {
        SFMLShapeType::RECTANGLE, SFMLShapeType::RECTANGLE,
        SFMLShapeType::RECTANGLE, SFMLShapeType::RECTANGLE,
        SFMLShapeType::RECTANGLE, SFMLShapeType::RECTANGLE,
        SFMLShapeType::RECTANGLE, SFMLShapeType::RECTANGLE,
        SFMLShapeType::RECTANGLE, SFMLShapeType::RECTANGLE,
        SFMLShapeType::RECTANGLE, SFMLShapeType::RECTANGLE,
        SFMLShapeType::RECTANGLE, SFMLShapeType::RECTANGLE,
        SFMLShapeType::RECTANGLE, SFMLShapeType::RECTANGLE,
        SFMLShapeType::RECTANGLE, SFMLShapeType::RECTANGLE,
        SFMLShapeType::RECTANGLE, SFMLShapeType::RECTANGLE,
        SFMLShapeType::RECTANGLE, SFMLShapeType::RECTANGLE,
        SFMLShapeType::RECTANGLE, SFMLShapeType::RECTANGLE,
        SFMLShapeType::HEXAGON,   SFMLShapeType::HEXAGON,
        SFMLShapeType::HEXAGON,
    };

    objTexturePaths.clear();
    for (int i = 0; i < objSizes.size(); ++i) {
      objTexturePaths.push_back("./assets/textures/material/stone.png");
    }

    l->setObjects(objSizes, objPositions, objColors, shapeTypes,
                  objTexturePaths);

    objSizes = {
        sf::Vector2f(bound_x, BLOCK * 2),
        // sides
        sf::Vector2f(BLOCK, BLOCK * 12),
        sf::Vector2f(BLOCK, BLOCK * 12),
        // Inside hole
        sf::Vector2f(BLOCK, BLOCK),
        sf::Vector2f(BLOCK, BLOCK),
    };
    objPositions = {
        {sf::Vector2f(0.0f, bound_y - 2 * BLOCK)},
        // sides
        {sf::Vector2f(START_LEVEL_X, START_LEVEL_Y - 12 * BLOCK)},
        {sf::Vector2f(START_LEVEL_X + 20 * BLOCK, START_LEVEL_Y - 12 * BLOCK)},
        // squares
        {
            sf::Vector2f(START_LEVEL_X + 1 * BLOCK, START_LEVEL_Y - 6 * BLOCK),
            sf::Vector2f(START_LEVEL_X + 4 * BLOCK, START_LEVEL_Y - BLOCK),
            sf::Vector2f(START_LEVEL_X + 4 * BLOCK, START_LEVEL_Y),
            sf::Vector2f(START_LEVEL_X + 1 * BLOCK, START_LEVEL_Y),
        },
        {
            sf::Vector2f(START_LEVEL_X + 20 * BLOCK, START_LEVEL_Y - 6 * BLOCK),
            sf::Vector2f(START_LEVEL_X + 17 * BLOCK, START_LEVEL_Y - BLOCK),
            sf::Vector2f(START_LEVEL_X + 20 * BLOCK, START_LEVEL_Y),
            sf::Vector2f(START_LEVEL_X + 17 * BLOCK, START_LEVEL_Y),
        },
    };
    objColors = {{120, 110, 100},
                 {120, 110, 100},
                 {120, 110, 100},
                 {120, 110, 100},
                 {120, 110, 100}};

    shapeTypes = {SFMLShapeType::RECTANGLE, SFMLShapeType::RECTANGLE,
                  SFMLShapeType::RECTANGLE, SFMLShapeType::CUSTOM,
                  SFMLShapeType::CUSTOM};
    for (int i = 0; i < objSizes.size(); ++i) {
        floorTexturePaths.push_back("./assets/textures/material/asphalt.png");
    }

    l->setFloor(objSizes, objPositions, objColors, shapeTypes, floorTexturePaths);

    objSizes = {
        sf::Vector2f(BLOCK / 2.0f, BLOCK / 2.0f),
        sf::Vector2f(2 * BLOCK / 2.0f, 2 * BLOCK / 2.0f),
        sf::Vector2f(BLOCK / 2.0f, BLOCK / 2.0f),
    };
    objPositions = {
        {sf::Vector2f(START_LEVEL_X + 6 * BLOCK, START_LEVEL_Y - 2 * BLOCK)},
        {sf::Vector2f(START_LEVEL_X + 10 * BLOCK, START_LEVEL_Y - 3 * BLOCK)},
        {sf::Vector2f(START_LEVEL_X + 14 * BLOCK, START_LEVEL_Y - 2 * BLOCK)},
    };
    objColors = {{9, 186, 45}, {9, 186, 45}, {9, 186, 45}};

    texturePaths.clear();
    for (int i = 0; i < objSizes.size(); ++i) {
      texturePaths.push_back("./assets/textures/multi/domingo_perez.png");
    }
    l->setTargets(objSizes, objPositions, objColors, texturePaths);

    break;

  case 4:
    // Level for testing the killing of birds
    // And triangles
    bound_x = 2000;
    bound_y = 1000;
    START_LEVEL_X = bound_x - BLOCK * 26;
    START_LEVEL_Y = bound_y - BLOCK * 2;

    l->setStarts(START_LEVEL_X, START_LEVEL_Y);

    l->setBounds(bound_x, bound_y);

    objSizes = {sf::Vector2f(BLOCK, BLOCK)};

    objPositions = {{sf::Vector2f(START_LEVEL_X, START_LEVEL_Y - BLOCK)}};

    objColors = {{0, 0, 0}};

    shapeTypes = {SFMLShapeType::TRIANGLE};

    objTexturePaths.clear();
    for (int i = 0; i < objSizes.size(); ++i) {
      objTexturePaths.push_back("./assets/textures/material/wood.png");
    }
    l->setObjects(objSizes, objPositions, objColors, shapeTypes,
                  objTexturePaths);

    objSizes = {
        sf::Vector2f(bound_x, BLOCK * 2),
        sf::Vector2f(BLOCK, BLOCK),
    };
    objPositions = {
        {sf::Vector2f(0.0f, bound_y - 2 * BLOCK)},
        // sides
        {sf::Vector2f(START_LEVEL_X + 8 * BLOCK, START_LEVEL_Y - 12 * BLOCK),
         sf::Vector2f(START_LEVEL_X + 14 * BLOCK, START_LEVEL_Y - 10 * BLOCK),
         sf::Vector2f(START_LEVEL_X + 8 * BLOCK, START_LEVEL_Y),
         sf::Vector2f(START_LEVEL_X + 15 * BLOCK, START_LEVEL_Y)},
    };
    objColors = {{120, 110, 100}, {120, 110, 100}};

    shapeTypes = {SFMLShapeType::RECTANGLE, SFMLShapeType::CUSTOM};

    for (int i = 0; i < objSizes.size(); ++i) {
        floorTexturePaths.push_back("./assets/textures/material/asphalt.png");
    }

    l->setFloor(objSizes, objPositions, objColors, shapeTypes, floorTexturePaths);

    objSizes = {
        sf::Vector2f(BLOCK / 2.0f, BLOCK / 2.0f),
    };
    objPositions = {
        {sf::Vector2f(START_LEVEL_X + 2 * BLOCK, START_LEVEL_Y - BLOCK)},
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
