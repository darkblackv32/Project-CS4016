#pragma once
#include <SFML/Graphics.hpp>

constexpr float GRAVEDAD = 0.1f;
constexpr float FUERZA_MULTIPLICADOR = 20.0f;
constexpr float RADIO_MAX_RESORTERA = 180.0f;
constexpr float COEF_RESTITUCION = 0.6f;
constexpr float FRICCION_SUELO = 0.7f;
constexpr float BLOCK = 30;
constexpr int OPTIONS_PER_PAGE = 9;
constexpr int OPTIONS_PER_ROW = 3;
constexpr float BUTTON_SCALE_X = 30.0f;
constexpr float BUTTON_SCALE_Y = 50.0f;
constexpr float SPACE_BETWEEN_ENDS_BUTTONS = 30.0f;
constexpr float ELASTICIDAD_BLOCK = 0.1f;
constexpr float STATIC_FRICTION_BLOCK = 0.8f;
constexpr float LINE_THICKNESS = 1.0f;
constexpr float BIRD_RADIUS = 30.0f;
constexpr float OFFSET_ABOVE = 1.0f;
constexpr float THRESHOLD_VELOCITY = 0.01f;
const sf::Color LINE_COLOR = {230, 230, 230};
const sf::Color DEFAULT_COLOR_OBJ = {0, 0, 0};
// Box2D works with meters, so we need a scale to convert pixels to meters
const float SCALE = 30.f;
const int SPRITE_WIDTH_WITH_PADDING = 250;
