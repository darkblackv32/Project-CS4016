#pragma once
#include <SFML/Graphics.hpp>
#include "Constants.h"
#include <cmath>

class Slingshot {
private:
    sf::Texture woodTexture;
    sf::Texture rubberTexture;
    
public:
    sf::RectangleShape posteIzq;
    sf::RectangleShape posteDer;
    sf::VertexArray bandaIzq;
    sf::VertexArray bandaDer;
    
    Slingshot();
    void loadTextures();
    void updateBands(const sf::Vector2f& birdPosition, bool isStretched);
    void draw(sf::RenderWindow& window);
};