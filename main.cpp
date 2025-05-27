#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>
#include "Bird.h"
#include "Slingshot.h"
#include "Physics.h"
#include "Constants.h"

int main() {
    sf::RenderWindow ventana(sf::VideoMode(800, 600), "Angry Birds Ultimate");
    ventana.setFramerateLimit(60);

    Bird pajaro;
    Slingshot resortera;
    bool arrastrando = false;
    sf::Vector2f clickOffset;
    std::vector<sf::Vector2f> trayectoria;

    sf::RectangleShape suelo({800.0f, 50.0f});
    suelo.setPosition(0.0f, 550.0f);
    suelo.setFillColor({120, 110, 100});

    sf::RectangleShape fondo({800.0f, 600.0f});
    fondo.setFillColor({135, 206, 235});

    while(ventana.isOpen()) {
        sf::Event evento;
        while(ventana.pollEvent(evento)) {
            if(evento.type == sf::Event::Closed) ventana.close();

            if(evento.type == sf::Event::MouseButtonPressed) {
                sf::Vector2f mousePos = ventana.mapPixelToCoords(sf::Mouse::getPosition(ventana));
                if(pajaro.figura.getGlobalBounds().contains(mousePos) && !pajaro.lanzado) {
                    arrastrando = true;
                    clickOffset = pajaro.figura.getPosition() - mousePos;
                }
            }

            if(evento.type == sf::Event::MouseButtonReleased && arrastrando) {
                arrastrando = false;
                pajaro.lanzado = true;
                
                sf::Vector2f direccion = POS_RESORTERA - pajaro.figura.getPosition();
                pajaro.velocidad = direccion * FUERZA_MULTIPLICADOR;
            }
        }

        if(arrastrando) {
            sf::Vector2f mousePos = ventana.mapPixelToCoords(sf::Mouse::getPosition(ventana));
            sf::Vector2f nuevaPos = mousePos + clickOffset;
            
            sf::Vector2f vectorResortera = nuevaPos - POS_RESORTERA;
            float distancia = std::hypot(vectorResortera.x, vectorResortera.y);
            
            if(distancia > RADIO_MAX_RESORTERA) {
                vectorResortera = (vectorResortera / distancia) * RADIO_MAX_RESORTERA;
                nuevaPos = POS_RESORTERA + vectorResortera;
            }
            
            pajaro.figura.setPosition(nuevaPos);
            
            sf::Vector2f velocidadInicial = (POS_RESORTERA - nuevaPos) * FUERZA_MULTIPLICADOR;
            trayectoria = Physics::calcularTrayectoria(nuevaPos, velocidadInicial);
        }

        if(pajaro.lanzado) {
            pajaro.updatePhysics(0.016f);
            Physics::handleGroundCollision(pajaro.figura, pajaro.velocidad, 550.0f);
            
            if(std::abs(pajaro.velocidad.x) < 15.0f && 
               std::abs(pajaro.velocidad.y) < 15.0f && 
               pajaro.figura.getPosition().y + 22.0f >= 550.0f - 22.0f) {
                pajaro.reset();
            }
        }

        ventana.clear();
        ventana.draw(fondo);
        ventana.draw(resortera.posteIzq);
        ventana.draw(resortera.posteDer);
        
        if(arrastrando || !pajaro.lanzado) {
            resortera.updateBands(pajaro.figura.getPosition());
            ventana.draw(resortera.bandaIzq);
            ventana.draw(resortera.bandaDer);
        }

        if(arrastrando) {
            for(const auto& punto : trayectoria) {
                sf::CircleShape puntoTrazo(3.0f);
                puntoTrazo.setFillColor(sf::Color(0, 0, 0, 120));
                puntoTrazo.setOrigin(1.5f, 1.5f);
                puntoTrazo.setPosition(punto);
                ventana.draw(puntoTrazo);
            }
        }

        ventana.draw(suelo);
        ventana.draw(pajaro.figura);
        ventana.display();
    }

    return 0;
}