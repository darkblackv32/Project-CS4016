#include "Slingshot.h"

Slingshot::Slingshot() {
    posteIzq.setSize({25.0f, 200.0f});
    posteIzq.setFillColor({139, 69, 19});
    posteIzq.setPosition(POS_RESORTERA.x - 50.0f, POS_RESORTERA.y - 80.0f);

    posteDer.setSize({25.0f, 200.0f});
    posteDer.setFillColor({139, 69, 19});
    posteDer.setOrigin(12.5f, 0.0f);
    posteDer.setPosition(POS_RESORTERA.x + 50.0f, POS_RESORTERA.y - 80.0f);

    bandaIzq.setPrimitiveType(sf::LinesStrip);
    bandaIzq.resize(2);
    bandaDer.setPrimitiveType(sf::LinesStrip);
    bandaDer.resize(2);
}

void Slingshot::updateBands(const sf::Vector2f& birdPosition) {
    sf::Vector2f anclajeIzq = posteIzq.getPosition() + sf::Vector2f(0.0f, 50.0f);
    sf::Vector2f anclajeDer = posteDer.getPosition() + sf::Vector2f(0.0f, 50.0f);

    bandaIzq[0].position = anclajeIzq;
    bandaIzq[1].position = birdPosition;
    bandaDer[0].position = anclajeDer;
    bandaDer[1].position = birdPosition;

    for(int i = 0; i < 2; ++i) {
        bandaIzq[i].color = sf::Color(80, 80, 80);
        bandaDer[i].color = sf::Color(80, 80, 80);
    }
}