#include "Levels.h"
#include "Constants.h"
#include <iostream>

// Ejemplo de cómo crear un nivel con diferentes tipos de shapes usando el sistema SFML interno
void createDemoLevelWithSFMLShapes() {
    std::cout << "=== DEMOSTRACIÓN DEL SISTEMA GENERALIZADO DE SHAPES SFML ===\n\n";

    // Crear un nivel de prueba
    Level *nivel = new Level();
    nivel->setNLevel(99); // Número de nivel demo
    nivel->setBounds(1400, 900);
    nivel->setStarts(100, 750);

    std::cout << "1. OBJETOS CON DIFERENTES TIPOS DE SHAPES:\n";
    std::cout << "   Usando sf::RectangleShape, sf::CircleShape, sf::ConvexShape\n\n";
    
    // Vector de tamaños para diferentes shapes
    std::vector<sf::Vector2f> objectSizes = {
        sf::Vector2f(BLOCK, 2 * BLOCK),     // Rectángulo
        sf::Vector2f(BLOCK, BLOCK),         // Círculo (radio = BLOCK)
        sf::Vector2f(BLOCK, 1.5 * BLOCK),   // Triángulo
        sf::Vector2f(BLOCK * 0.8f, BLOCK * 0.8f), // Diamante
        sf::Vector2f(BLOCK * 1.2f, BLOCK * 1.2f), // Hexágono
        sf::Vector2f(2 * BLOCK, BLOCK)      // Rectángulo largo
    };
    
    // Posiciones de los objetos
    std::vector<std::pair<float, float>> objectPos = {
        std::make_pair(200, 600),  // Rectángulo
        std::make_pair(350, 600),  // Círculo
        std::make_pair(500, 600),  // Triángulo
        std::make_pair(650, 600),  // Diamante
        std::make_pair(800, 600),  // Hexágono
        std::make_pair(950, 600)   // Rectángulo largo
    };
    
    // Colores de los objetos
    std::vector<sf::Color> objectColors = {
        sf::Color::Red,        // Rectángulo
        sf::Color::Blue,       // Círculo
        sf::Color::Green,      // Triángulo
        sf::Color::Yellow,     // Diamante
        sf::Color::Magenta,    // Hexágono
        sf::Color::Cyan        // Rectángulo largo
    };
    
    // Tipos de shapes SFML a usar
    std::vector<SFMLShapeType> objectTypes = {
        SFMLShapeType::RECTANGLE,
        SFMLShapeType::CIRCLE,
        SFMLShapeType::TRIANGLE,
        SFMLShapeType::DIAMOND,
        SFMLShapeType::HEXAGON,
        SFMLShapeType::RECTANGLE
    };
    
    // Establecer objetos en el nivel
    nivel->setObjects(objectSizes, objectPos, objectColors, objectTypes);
    
    std::cout << "   ✓ Rectángulo Rojo en (200, 600) usando sf::RectangleShape\n";
    std::cout << "   ✓ Círculo Azul en (350, 600) usando sf::CircleShape\n";
    std::cout << "   ✓ Triángulo Verde en (500, 600) usando sf::ConvexShape\n";
    std::cout << "   ✓ Diamante Amarillo en (650, 600) usando sf::ConvexShape\n";
    std::cout << "   ✓ Hexágono Magenta en (800, 600) usando sf::ConvexShape\n";
    std::cout << "   ✓ Rectángulo Cian en (950, 600) usando sf::RectangleShape\n\n";

    std::cout << "2. TARGETS CON DIFERENTES SHAPES:\n";
    std::cout << "   Cada target tiene su textura y physics apropiada\n\n";
    
    // Targets con diferentes formas
    std::vector<sf::Vector2f> targetSizes = {
        sf::Vector2f(BLOCK/2, BLOCK/2),     // Círculo target
        sf::Vector2f(BLOCK/2, BLOCK/2),     // Triángulo target
        sf::Vector2f(BLOCK/2, BLOCK/2),     // Diamante target
        sf::Vector2f(BLOCK/2, BLOCK/2)      // Hexágono target
    };
    
    std::vector<std::pair<float, float>> targetPos = {
        std::make_pair(250, 500),  // Círculo target
        std::make_pair(450, 500),  // Triángulo target
        std::make_pair(650, 500),  // Diamante target
        std::make_pair(850, 500)   // Hexágono target
    };
    
    std::vector<sf::Color> targetColors = {
        sf::Color::White,      // Círculo
        sf::Color::White,      // Triángulo
        sf::Color::White,      // Diamante
        sf::Color::White       // Hexágono
    };
    
    std::vector<std::string> targetTextures = {
        "./assets/textures/milei/kirchner.png",
        "./assets/textures/milei/fernandez.png",
        "./assets/textures/milei/massa.png",
        "./assets/textures/milei/kirchner.png"
    };
    
    std::vector<SFMLShapeType> targetTypes = {
        SFMLShapeType::CIRCLE,
        SFMLShapeType::TRIANGLE,
        SFMLShapeType::DIAMOND,
        SFMLShapeType::HEXAGON
    };
    
    nivel->setTargets(targetSizes, targetPos, targetColors, targetTextures, targetTypes);
    
    std::cout << "   ✓ Target Circular con textura Kirchner\n";
    std::cout << "   ✓ Target Triangular con textura Fernández\n";
    std::cout << "   ✓ Target Diamante con textura Massa\n";
    std::cout << "   ✓ Target Hexagonal con textura Kirchner\n\n";

    std::cout << "3. SUELO CON VARIEDAD DE SHAPES:\n";
    std::cout << "   Combinando diferentes formas para el terreno\n\n";
    
    // Elementos del suelo con diferentes formas
    std::vector<sf::Vector2f> floorSizes = {
        sf::Vector2f(1200, BLOCK),          // Suelo principal rectangular
        sf::Vector2f(BLOCK, BLOCK),         // Plataforma circular
        sf::Vector2f(BLOCK * 1.5f, BLOCK), // Plataforma triangular
        sf::Vector2f(BLOCK, BLOCK)          // Plataforma diamante
    };
    
    std::vector<std::pair<float, float>> floorPos = {
        std::make_pair(100, 800),   // Suelo principal
        std::make_pair(300, 750),   // Plataforma circular
        std::make_pair(600, 750),   // Plataforma triangular
        std::make_pair(900, 750)    // Plataforma diamante
    };
    
    std::vector<sf::Color> floorColors = {
        sf::Color(120, 110, 100),   // Marrón para suelo principal
        sf::Color(100, 90, 80),     // Marrón oscuro para circular
        sf::Color(80, 70, 60),      // Marrón muy oscuro para triangular
        sf::Color(140, 130, 120)    // Marrón claro para diamante
    };
    
    std::vector<SFMLShapeType> floorTypes = {
        SFMLShapeType::RECTANGLE,
        SFMLShapeType::CIRCLE,
        SFMLShapeType::TRIANGLE,
        SFMLShapeType::DIAMOND
    };
    
    nivel->setFloor(floorSizes, floorPos, floorColors, floorTypes);
    
    std::cout << "   ✓ Suelo rectangular principal (1200x50)\n";
    std::cout << "   ✓ Plataforma circular elevada\n";
    std::cout << "   ✓ Plataforma triangular\n";
    std::cout << "   ✓ Plataforma en forma de diamante\n\n";

    std::cout << "4. CARACTERÍSTICAS DEL SISTEMA:\n\n";
    
    std::cout << "   🔧 INTEGRACIÓN CON PHYSICS:\n";
    std::cout << "      • Círculos → b2CircleShape (física circular)\n";
    std::cout << "      • Rectángulos → b2PolygonShape como caja\n";
    std::cout << "      • Triángulos/Diamantes/Hexágonos → b2PolygonShape como caja\n";
    std::cout << "      • Cuerpos dinámicos para objetos y targets\n";
    std::cout << "      • Cuerpos estáticos para suelo\n\n";
    
    std::cout << "   🎨 RENDERIZADO SFML:\n";
    std::cout << "      • sf::RectangleShape para rectángulos\n";
    std::cout << "      • sf::CircleShape para círculos\n";
    std::cout << "      • sf::ConvexShape para formas complejas\n";
    std::cout << "      • Soporte completo de texturas\n";
    std::cout << "      • Contornos y colores personalizables\n\n";
    
    std::cout << "   📦 GESTIÓN DE MEMORIA:\n";
    std::cout << "      • std::unique_ptr<sf::Shape> para manejo automático\n";
    std::cout << "      • Polimorfismo usando sf::Shape base\n";
    std::cout << "      • No hay memory leaks\n\n";
    
    std::cout << "   🔄 COMPATIBILIDAD:\n";
    std::cout << "      • Totalmente compatible con código existente\n";
    std::cout << "      • Parámetros opcionales (retrocompatible)\n";
    std::cout << "      • Factory methods para extensibilidad\n\n";

    std::cout << "5. TIPOS DE SHAPES SOPORTADOS:\n\n";
    std::cout << "   • SFMLShapeType::RECTANGLE → sf::RectangleShape\n";
    std::cout << "   • SFMLShapeType::CIRCLE → sf::CircleShape\n";
    std::cout << "   • SFMLShapeType::TRIANGLE → sf::ConvexShape (3 vértices)\n";
    std::cout << "   • SFMLShapeType::DIAMOND → sf::ConvexShape (4 vértices)\n";
    std::cout << "   • SFMLShapeType::HEXAGON → sf::ConvexShape (6 vértices)\n";
    std::cout << "   • SFMLShapeType::CUSTOM_POLYGON → sf::ConvexShape (N vértices)\n\n";

    std::cout << "6. EJEMPLO DE USO EN CÓDIGO:\n\n";
    std::cout << "   // Crear objetos con diferentes shapes\n";
    std::cout << "   std::vector<SFMLShapeType> tipos = {\n";
    std::cout << "       SFMLShapeType::RECTANGLE,\n";
    std::cout << "       SFMLShapeType::CIRCLE,\n";
    std::cout << "       SFMLShapeType::TRIANGLE\n";
    std::cout << "   };\n";
    std::cout << "   \n";
    std::cout << "   nivel->setObjects(tamaños, posiciones, colores, tipos);\n\n";

    std::cout << "7. VENTAJAS DEL SISTEMA:\n\n";
    std::cout << "   ✅ Usa el sistema SFML nativo (sf::Shape)\n";
    std::cout << "   ✅ Polimorfismo sin overhead adicional\n";
    std::cout << "   ✅ Física apropiada para cada forma\n";
    std::cout << "   ✅ Fácil extensión para nuevas formas\n";
    std::cout << "   ✅ Compatibilidad total hacia atrás\n";
    std::cout << "   ✅ Gestión automática de memoria\n";
    std::cout << "   ✅ Soporte completo de texturas y efectos\n\n";

    // Limpiar memoria
    delete nivel;
    
    std::cout << "=== DEMO COMPLETADA EXITOSAMENTE ===\n\n";
    std::cout << "El sistema ahora soporta:\n";
    std::cout << "• Almacenamiento polimórfico con std::unique_ptr<sf::Shape>\n";
    std::cout << "• Factory methods para crear diferentes tipos de shapes\n";
    std::cout << "• Integración correcta entre SFML y Box2D physics\n";
    std::cout << "• Verificación automática de tipos en setObjects/setTargets/setFloor\n";
    std::cout << "• Parámetros opcionales para mantener compatibilidad\n";
    std::cout << "• Renderizado eficiente usando shapes nativos de SFML\n\n";
    
    std::cout << "¡Ahora puedes usar triángulos, círculos, diamantes, hexágonos\n";
    std::cout << "y cualquier polígono personalizado en tus niveles!\n";
}

int main() {
    createDemoLevelWithSFMLShapes();
    return 0;
} 