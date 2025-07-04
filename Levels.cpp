#include "Levels.h"
#include "Constants.h"
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
                       std::vector<sf::Color> &objectColors) {

  std::vector<sf::RectangleShape> objects;
  for (int i = 0; i < objectSizes.size(); i++) {
    // Physics
    b2Body *temp_body = createBox(objectPos[i].first, objectPos[i].first,
                                  objectSizes[i].x / 2, objectSizes[i].y / 2);
    // SFML
    b2Fixture *fixture = temp_body->GetFixtureList();
    b2Shape::Type shapeType = fixture->GetType();
    sf::Vector2f pos = metersToPixels(temp_body->GetPosition());
    float angle = temp_body->GetAngle() * 180.f / b2_pi;

    b2PolygonShape *poly = (b2PolygonShape *)fixture->GetShape();
    // Asumimos que es una caja creada con SetAsBox
    b2Vec2 halfSize = poly->m_vertices[2];

    sf::RectangleShape temp(
        sf::Vector2f(halfSize.x * 2 * SCALE, halfSize.y * 2 * SCALE));
    temp.setOrigin(temp.getSize().x / 2.f, temp.getSize().y / 2.f);
    temp.setPosition(pos);
    temp.setRotation(angle);
    temp.setFillColor(objectColors[i]);
    temp.setOutlineColor({170, 143, 24});
    temp.setOutlineThickness(LINE_THICKNESS);

    objects.push_back(temp);
  }

  this->objects = objects;
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
                       std::vector<std::pair<float, float>> &objectPos,
                       std::vector<sf::Color> &objectColors) {

  std::vector<sf::RectangleShape> objects;
  for (int i = 0; i < objectSizes.size(); i++) {
    // SFML
    sf::RectangleShape temp(objectSizes[i]);
    temp.setPosition(objectPos[i].first, objectPos[i].second);
    temp.setFillColor(objectColors[i]);

    objects.push_back(temp);

    // Physics
    // TODO
  }

  this->targets = objects;
}

void Level::setFloor(std::vector<sf::Vector2f> &objectSizes,
                     std::vector<std::pair<float, float>> &objectPos,
                     std::vector<sf::Color> &objectColors) {

  std::vector<sf::RectangleShape> objects;
  for (int i = 0; i < objectSizes.size(); i++) {
    // Physics
    b2Body *temp_static = nullptr;
    b2BodyDef groundBodyDef;
    // Consider the conversion from origin point at the top left to the center
    groundBodyDef.position.Set(
        (objectPos[i].first + objectSizes[i].x / 2.0f) / SCALE,
        (objectPos[i].second + objectSizes[i].y / 2.0f) / SCALE);
    temp_static = m_physics.CreateBody(&groundBodyDef);
    b2PolygonShape groundBox;
    groundBox.SetAsBox(objectSizes[i].x / 2.f / SCALE,
                       objectSizes[i].y / SCALE);
    temp_static->CreateFixture(&groundBox, 0.0f);
    m_static.push_back(temp_static);

    // SFML
    sf::RectangleShape temp(objectSizes[i]);
    temp.setPosition(objectPos[i].first, objectPos[i].second);
    // temp.setPosition(objectPos[i].first, objectPos[i].second);
    temp.setPosition(objectPos[i].first, objectPos[i].second);
    temp.setFillColor(objectColors[i]);

    objects.push_back(temp);
  }

  this->floor = objects;
}

void Level::setStarts(float x, float y) {
  this->START_LEVEL_X = x;
  this->START_LEVEL_Y = y;
}

void Level::setBounds(float x, float y) {
  this->x_bound = x;
  this->y_bound = y;
}

void Level::render(sf::RenderWindow &ventana) {
  // Renders the objects and also updates their positions according to the
  // physics bodies
  for (int i = 0; i < this->floor.size(); i++) {
    ventana.draw(this->floor[i]);
  }

  for (int i = 0; i < this->m_bodies.size(); i++) {
    b2Body *body = m_bodies[i];
    b2Fixture *fixture = body->GetFixtureList();
    while (fixture) {
      b2Shape::Type shapeType = fixture->GetType();
      sf::Vector2f pos = metersToPixels(body->GetPosition());
      float angle = body->GetAngle() * 180.f / b2_pi;

      b2PolygonShape *poly = (b2PolygonShape *)fixture->GetShape();
      // Asumimos que es una caja creada con SetAsBox
      b2Vec2 halfSize = poly->m_vertices[2];

      sf::RectangleShape rect = objects[i];
      rect.setPosition(pos);
      rect.setRotation(angle);
      ventana.draw(rect);

      fixture = fixture->GetNext();
    }
  }

  for (int i = 0; i < this->targets.size(); i++) {
    ventana.draw(this->targets[i]);
    // TODO
    // Also consider they can disappear
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

Level *return_level(int level, int width, int height) {
  std::vector<sf::Vector2f> objSizes;
  std::vector<std::pair<float, float>> objPositions;
  std::vector<sf::Color> objColors;
  Level *l = new Level();
  float START_LEVEL_X;
  float START_LEVEL_Y;
  int bound_x;
  int bound_y;

  switch (level) {
  case 0:
    bound_x = 1000;
    bound_y = 700;
    START_LEVEL_X = bound_x / 2.0f;
    START_LEVEL_Y = bound_y - BLOCK * 3;

    objSizes = {
        sf::Vector2f(BLOCK, 2 * BLOCK), sf::Vector2f(6 * BLOCK, BLOCK),
        sf::Vector2f(6 * BLOCK, BLOCK), sf::Vector2f(BLOCK, 2 * BLOCK),
        sf::Vector2f(BLOCK, 7 * BLOCK), sf::Vector2f(BLOCK, 7 * BLOCK),
        sf::Vector2f(BLOCK * 6, BLOCK), sf::Vector2f(BLOCK, 7 * BLOCK),
        sf::Vector2f(BLOCK, 7 * BLOCK), sf::Vector2f(4 * BLOCK, BLOCK),
    };

    objPositions = {
        std::make_pair(START_LEVEL_X, START_LEVEL_Y - 2 * BLOCK),
        std::make_pair(START_LEVEL_X, START_LEVEL_Y),
        std::make_pair(START_LEVEL_X + 6 * BLOCK, START_LEVEL_Y),
        std::make_pair(START_LEVEL_X + 11 * BLOCK, START_LEVEL_Y - 2 * BLOCK),
        std::make_pair(START_LEVEL_X + 3 * BLOCK, START_LEVEL_Y - 7 * BLOCK),
        std::make_pair(START_LEVEL_X + 8 * BLOCK, START_LEVEL_Y - 7 * BLOCK),
        std::make_pair(START_LEVEL_X + 3 * BLOCK, START_LEVEL_Y - 8 * BLOCK),
        std::make_pair(START_LEVEL_X + 4 * BLOCK, START_LEVEL_Y - 15 * BLOCK),
        std::make_pair(START_LEVEL_X + 7 * BLOCK, START_LEVEL_Y - 15 * BLOCK),
        std::make_pair(START_LEVEL_X + 4 * BLOCK, START_LEVEL_Y - 16 * BLOCK),
    };

    objColors = {
        {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0},
        {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0},
    };

    l->setObjects(objSizes, objPositions, objColors);

    objSizes = {sf::Vector2f(bound_x, BLOCK * 2)};
    objPositions = {std::make_pair(0.0f, bound_y - 2 * BLOCK)};
    objColors = {{120, 110, 100}};

    l->setFloor(objSizes, objPositions, objColors);

    objSizes = {sf::Vector2f(BLOCK, BLOCK)};
    objPositions = {
        std::make_pair(START_LEVEL_X + 5.5 * BLOCK, START_LEVEL_Y - BLOCK)};
    objColors = {{9, 186, 45}};

    l->setTargets(objSizes, objPositions, objColors);

    l->setBounds(bound_x, bound_y);

    break;
  case 1:
    bound_x = 2000;
    bound_y = 800;
    START_LEVEL_X = bound_x - BLOCK * 27;
    START_LEVEL_Y = bound_y - BLOCK * 6;

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
                sf::Vector2f(BLOCK, BLOCK),        sf::Vector2f(BLOCK, BLOCK),
                sf::Vector2f(BLOCK, BLOCK),        sf::Vector2f(BLOCK, BLOCK),
                sf::Vector2f(BLOCK, BLOCK),        sf::Vector2f(BLOCK, BLOCK),
                sf::Vector2f(2 * BLOCK, 2 * BLOCK)};
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

    l->setTargets(objSizes, objPositions, objColors);

    l->setStarts(START_LEVEL_X, START_LEVEL_Y);

    l->setBounds(bound_x, bound_y);

    break;
  case 2:
    bound_x = 1500;
    bound_y = 900;
    START_LEVEL_X = bound_x - BLOCK * 27;
    START_LEVEL_Y = bound_y - BLOCK * 2;

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

    objSizes = {sf::Vector2f(bound_x, BLOCK * 2),
                sf::Vector2f(BLOCK * 5, BLOCK * 2),
                sf::Vector2f(BLOCK * 5, BLOCK * 4)};
    objPositions = {
        std::make_pair(0.0f, bound_y - 2 * BLOCK),
        std::make_pair(START_LEVEL_X + 5.5 * BLOCK, START_LEVEL_Y - 2 * BLOCK),
        std::make_pair(START_LEVEL_X + 11.5 * BLOCK,
                       START_LEVEL_Y - 4 * BLOCK)};
    objColors = {{120, 110, 100}, {120, 110, 100}, {120, 110, 100}};

    l->setFloor(objSizes, objPositions, objColors);

    objSizes = {// inside
                sf::Vector2f(BLOCK, BLOCK), sf::Vector2f(BLOCK, BLOCK),
                sf::Vector2f(BLOCK, BLOCK)};
    objPositions = {
        // inside
        std::make_pair(START_LEVEL_X + 1.5 * BLOCK, START_LEVEL_Y - 2 * BLOCK),
        std::make_pair(START_LEVEL_X + 7.5 * BLOCK, START_LEVEL_Y - 4 * BLOCK),
        std::make_pair(START_LEVEL_X + 13.5 * BLOCK, START_LEVEL_Y - 6 * BLOCK),
    };
    objColors = {{9, 186, 45}, {9, 186, 45}, {9, 186, 45}};

    l->setTargets(objSizes, objPositions, objColors);

    l->setStarts(START_LEVEL_X, START_LEVEL_Y);

    l->setBounds(bound_x, bound_y);

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
