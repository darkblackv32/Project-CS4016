#pragma once
#include "Constants.h"
#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>

class Slingshot {
private:
    static constexpr float POST_WIDTH = 32.0f;
    static constexpr float POST_HEIGHT = 225.0f;
    static constexpr float BAND_THICKNESS = 12.0f;
    static constexpr float SAG_AMOUNT = 20.0f;

    sf::RectangleShape leftPost;
    sf::RectangleShape rightPost;

    sf::VertexArray leftBand;
    sf::VertexArray rightBand;

    sf::VertexArray leftVine;
    sf::VertexArray rightVine;

    sf::Clock animationClock;

    sf::Vector2f pos_resortera;

    static sf::Texture createAncientWoodTexture();

    void configurePosts();
    void configureBands();
    void configureVines();
    void updateBandGeometry(sf::VertexArray &band, const sf::Vector2f &start,
                            const sf::Vector2f &end);

public:
    Slingshot();
    Slingshot(sf::Vector2f &pr);
    void updateBands(const sf::Vector2f &birdPosition, bool isStretched);
    void draw(sf::RenderWindow &window);
};
