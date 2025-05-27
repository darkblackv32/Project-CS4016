#include "Physics.h"

std::vector<sf::Vector2f> Physics::calcularTrayectoria(sf::Vector2f inicio, sf::Vector2f velocidad) {
    std::vector<sf::Vector2f> puntos;
    const int pasos = 100;
    float dt = 0.05f;
    
    for(int i = 0; i < pasos; ++i) {
        float t = dt * i;
        sf::Vector2f pos(
            inicio.x + velocidad.x * t,
            inicio.y + velocidad.y * t + 0.5f * GRAVEDAD * t * t
        );
        
        if(pos.y > 550.0f) break;
        puntos.push_back(pos);
    }
    return puntos;
}

void Physics::handleGroundCollision(sf::CircleShape& figura, 
                                   sf::Vector2f& velocidad, 
                                   float groundLevel) {
    if(figura.getPosition().y + figura.getRadius() > groundLevel) {
        figura.setPosition(figura.getPosition().x, groundLevel - figura.getRadius());
        velocidad.y *= -COEF_RESTITUCION;
        velocidad.x *= FRICCION_SUELO;
    }
}