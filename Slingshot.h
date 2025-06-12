#pragma once
#include "Constants.h"
#include <cmath>
#include <SFML/Graphics.hpp>

class Slingshot {
private:
    static constexpr float POST_WIDTH = 30.0f;
    static constexpr float POST_HEIGHT = 220.0f;
    static constexpr float BAND_THICKNESS = 5.0f;
    static constexpr float SAG_AMOUNT = 20.0f;

    sf::RectangleShape leftPost;
    sf::RectangleShape rightPost;
    sf::VertexArray leftBand;
    sf::VertexArray rightBand;

    static sf::Texture createWoodTexture();
    void configurePosts();
    void configureBands();
    void updateBandGeometry(sf::VertexArray& band, const sf::Vector2f& start, 
                           const sf::Vector2f& end, bool isStretched);

public:
    Slingshot();
    void updateBands(const sf::Vector2f& birdPosition, bool isStretched);
    void draw(sf::RenderWindow& window) const;
};