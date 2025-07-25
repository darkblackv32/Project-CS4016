#pragma once
#include <SFML/Graphics.hpp>

constexpr float GRAVEDAD = 0.1f;
constexpr float FUERZA_MULTIPLICADOR = 20.0f;
constexpr float RADIO_MAX_RESORTERA = 180.0f;
constexpr float COEF_RESTITUCION = 0.6f;
constexpr float FRICCION_SUELO = 0.7f;
constexpr float BLOCK = 30;
constexpr int OPTIONS_PER_PAGE =
    1; // Show one level at a time for arrow navigation
constexpr int OPTIONS_PER_ROW = 3;
constexpr float BUTTON_SCALE_X = 30.0f;
constexpr float BUTTON_SCALE_Y = 50.0f;
constexpr float SPACE_BETWEEN_ENDS_BUTTONS = 30.0f;
constexpr float ELASTICIDAD_BLOCK = 0.1f;
constexpr float STATIC_FRICTION_BLOCK = 0.8f;
constexpr float LINE_THICKNESS = 1.0f;
constexpr float OFFSET_ABOVE = 1.0f;
constexpr float THRESHOLD_VELOCITY = 0.02f;
const sf::Color LINE_COLOR = {230, 230, 230};
const sf::Color DEFAULT_COLOR_OBJ = {0, 0, 0};
// Box2D works with meters, so we need a scale to convert pixels to meters
const float SCALE = 30.f;
const int SPRITE_WIDTH_WITH_PADDING = 250;
const float EXTRA_IMPULSE_DOWN = 300.0f;
const float EXTRA_IMPULSE_RIGHT = 200.0f;
const float OBJECT_LIFE = 170.0f;
const float OBJECT_DEFENSE = 20.0f;
const float TARGET_LIFE = 20.0f;
const float TARGET_DEFENSE = 4.0f;
// 0.1 percent of a impulse is damage
const float DAMAGE_MULTIPLIER = 0.2f;
const float TRAJECTORY_STEP = 0.0001f;
const int MAX_TRAJECTORY_POINTS = 50;
