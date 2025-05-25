#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>

// Constantes del juego
const float GRAVEDAD = 980.0f;
const float FUERZA_MULTIPLICADOR = 35.0f;
const float RADIO_MAX_RESORTERA = 180.0f;
const float COEF_RESTITUCION = 0.6f;
const float FRICCION_SUELO = 0.7f;
const sf::Vector2f POS_RESORTERA(100.0f, 470.0f); 

struct Pajaro {
    sf::CircleShape figura;
    sf::Vector2f velocidad;
    bool lanzado = false;
    bool enResortera = true;
};

struct Resortera {
    sf::RectangleShape posteIzq;
    sf::RectangleShape posteDer;
    sf::VertexArray bandaIzq;
    sf::VertexArray bandaDer;
    
    Resortera() {
        posteIzq.setSize({25.0f, 200.0f});
        posteIzq.setFillColor({139, 69, 19}); 
        posteIzq.setOrigin(0.0f, 0.0f);
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
};

std::vector<sf::Vector2f> CalcularTrayectoria(sf::Vector2f inicio, sf::Vector2f velocidad) {
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

void ActualizarBandas(Resortera& resortera, const sf::Vector2f& posPajaro) {
    sf::Vector2f anclajeIzq = resortera.posteIzq.getPosition() + sf::Vector2f(0.0f, 50.0f);
    sf::Vector2f anclajeDer = resortera.posteDer.getPosition() + sf::Vector2f(0.0f, 50.0f);
    
    resortera.bandaIzq[0].position = anclajeIzq;
    resortera.bandaIzq[1].position = posPajaro;
    resortera.bandaDer[0].position = anclajeDer;
    resortera.bandaDer[1].position = posPajaro;
    
    for(int i = 0; i < 2; ++i) {
        resortera.bandaIzq[i].color = sf::Color(80, 80, 80);
        resortera.bandaDer[i].color = sf::Color(80, 80, 80);
    }
}

int main() {
    sf::RenderWindow ventana(sf::VideoMode(800, 600), "Angry Birds Ultimate");
    ventana.setFramerateLimit(60);

    Pajaro pajaro;
    pajaro.figura.setRadius(22.0f);
    pajaro.figura.setFillColor(sf::Color(255, 60, 60));
    pajaro.figura.setOrigin(22.0f, 22.0f);
    pajaro.figura.setPosition(POS_RESORTERA);

    Resortera resortera;
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
            trayectoria = CalcularTrayectoria(nuevaPos, velocidadInicial);
        }

        if(pajaro.lanzado) {
            pajaro.velocidad.y += GRAVEDAD * 0.016f;
            pajaro.figura.move(pajaro.velocidad * 0.016f);

            if(pajaro.figura.getPosition().y + 22.0f > 550.0f) {
                pajaro.figura.setPosition(pajaro.figura.getPosition().x, 550.0f - 22.0f);
                pajaro.velocidad.y *= -COEF_RESTITUCION;
                pajaro.velocidad.x *= FRICCION_SUELO;
                
                if(std::abs(pajaro.velocidad.x) < 15.0f && std::abs(pajaro.velocidad.y) < 15.0f) {
                    pajaro.lanzado = false;
                    pajaro.figura.setPosition(POS_RESORTERA);
                }
            }
        }

        ventana.clear();
        
        ventana.draw(fondo);
        
        ventana.draw(resortera.posteIzq);
        ventana.draw(resortera.posteDer);
        
        if(arrastrando || !pajaro.lanzado) {
            ActualizarBandas(resortera, pajaro.figura.getPosition());
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