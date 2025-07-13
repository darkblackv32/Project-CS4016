#include "Levels.h"
#include "Constants.h"

// Ejemplo práctico: Modificar el nivel 0 para usar diferentes tipos de shapes
Level* create_mixed_shapes_level() {
    Level *l = new Level();
    l->setNLevel(0);
    
    int bound_x = 1200;
    int bound_y = 700;
    float START_LEVEL_X = bound_x - BLOCK * 19;
    float START_LEVEL_Y = bound_y - BLOCK * 3;

    l->setStarts(START_LEVEL_X, START_LEVEL_Y);
    l->setBounds(bound_x, bound_y);

    // OBJETOS CON FORMAS MIXTAS
    std::vector<sf::Vector2f> objSizes = {
        sf::Vector2f(BLOCK, 2 * BLOCK),     // 1. Pilar rectangular
        sf::Vector2f(6 * BLOCK, BLOCK),     // 2. Viga rectangular  
        sf::Vector2f(BLOCK, BLOCK),         // 3. Círculo decorativo
        sf::Vector2f(BLOCK, 2 * BLOCK),     // 4. Pilar rectangular
        sf::Vector2f(BLOCK * 1.5f, BLOCK * 1.5f), // 5. Triángulo estructural
        sf::Vector2f(BLOCK, BLOCK),         // 6. Diamante decorativo
        sf::Vector2f(6 * BLOCK, BLOCK),     // 7. Viga superior
        sf::Vector2f(BLOCK, BLOCK),         // 8. Hexágono especial
        sf::Vector2f(BLOCK, 2 * BLOCK),     // 9. Pilar final
        sf::Vector2f(4 * BLOCK, BLOCK),     // 10. Techo rectangular
    };

    std::vector<std::pair<float, float>> objPositions = {
        std::make_pair(START_LEVEL_X, START_LEVEL_Y - 2 * BLOCK - OFFSET_ABOVE),     // 1. Pilar izq
        std::make_pair(START_LEVEL_X, START_LEVEL_Y - OFFSET_ABOVE),                 // 2. Viga base
        std::make_pair(START_LEVEL_X + 7 * BLOCK, START_LEVEL_Y - OFFSET_ABOVE),     // 3. Círculo
        std::make_pair(START_LEVEL_X + 11 * BLOCK, START_LEVEL_Y - 2 * BLOCK - OFFSET_ABOVE), // 4. Pilar der
        std::make_pair(START_LEVEL_X + 3 * BLOCK, START_LEVEL_Y - 4 * BLOCK - OFFSET_ABOVE),  // 5. Triángulo
        std::make_pair(START_LEVEL_X + 8 * BLOCK, START_LEVEL_Y - 3 * BLOCK - OFFSET_ABOVE),  // 6. Diamante
        std::make_pair(START_LEVEL_X + 3 * BLOCK, START_LEVEL_Y - 8 * BLOCK - OFFSET_ABOVE),  // 7. Viga sup
        std::make_pair(START_LEVEL_X + 6 * BLOCK, START_LEVEL_Y - 9 * BLOCK - OFFSET_ABOVE),  // 8. Hexágono
        std::make_pair(START_LEVEL_X + 4 * BLOCK, START_LEVEL_Y - 15 * BLOCK - OFFSET_ABOVE), // 9. Pilar alto
        std::make_pair(START_LEVEL_X + 4 * BLOCK, START_LEVEL_Y - 16 * BLOCK - OFFSET_ABOVE), // 10. Techo
    };

    std::vector<sf::Color> objColors = {
        sf::Color(139, 69, 19),     // 1. Marrón madera
        sf::Color(139, 69, 19),     // 2. Marrón madera  
        sf::Color(255, 215, 0),     // 3. Dorado (decorativo)
        sf::Color(139, 69, 19),     // 4. Marrón madera
        sf::Color(34, 139, 34),     // 5. Verde bosque
        sf::Color(220, 20, 60),     // 6. Carmesí (especial)
        sf::Color(139, 69, 19),     // 7. Marrón madera
        sf::Color(138, 43, 226),    // 8. Violeta (único)
        sf::Color(139, 69, 19),     // 9. Marrón madera
        sf::Color(160, 82, 45),     // 10. Marrón claro
    };

    // TIPOS DE SHAPES PARA CADA OBJETO
    std::vector<SFMLShapeType> objTypes = {
        SFMLShapeType::RECTANGLE,   // 1. Pilar
        SFMLShapeType::RECTANGLE,   // 2. Viga  
        SFMLShapeType::CIRCLE,      // 3. Elemento decorativo circular
        SFMLShapeType::RECTANGLE,   // 4. Pilar
        SFMLShapeType::TRIANGLE,    // 5. Elemento triangular
        SFMLShapeType::DIAMOND,     // 6. Diamante decorativo
        SFMLShapeType::RECTANGLE,   // 7. Viga
        SFMLShapeType::HEXAGON,     // 8. Elemento hexagonal especial
        SFMLShapeType::RECTANGLE,   // 9. Pilar
        SFMLShapeType::RECTANGLE,   // 10. Techo
    };

    l->setObjects(objSizes, objPositions, objColors, objTypes);

    // SUELO CON VARIACIONES
    std::vector<sf::Vector2f> floorSizes = {
        sf::Vector2f(bound_x, BLOCK * 2),    // Suelo principal
        sf::Vector2f(BLOCK, BLOCK),          // Elemento circular
        sf::Vector2f(BLOCK, BLOCK)           // Elemento diamante
    };
    
    std::vector<std::pair<float, float>> floorPositions = {
        std::make_pair(0.0f, bound_y - 2 * BLOCK),
        std::make_pair(START_LEVEL_X - 2 * BLOCK, bound_y - 3 * BLOCK),
        std::make_pair(START_LEVEL_X + 15 * BLOCK, bound_y - 3 * BLOCK)
    };
    
    std::vector<sf::Color> floorColors = {
        sf::Color(120, 110, 100),
        sf::Color(100, 90, 80),
        sf::Color(80, 70, 60)
    };
    
    std::vector<SFMLShapeType> floorTypes = {
        SFMLShapeType::RECTANGLE,
        SFMLShapeType::CIRCLE,
        SFMLShapeType::DIAMOND
    };

    l->setFloor(floorSizes, floorPositions, floorColors, floorTypes);

    // TARGETS CON DIFERENTES FORMAS
    std::vector<sf::Vector2f> targetSizes = {
        sf::Vector2f(BLOCK / 2.0f, BLOCK / 2.0f),    // Target circular
        sf::Vector2f(BLOCK / 2.0f, BLOCK / 2.0f),    // Target triangular
        sf::Vector2f(BLOCK / 2.0f, BLOCK / 2.0f)     // Target hexagonal
    };
    
    std::vector<std::pair<float, float>> targetPositions = {
        std::make_pair(START_LEVEL_X + 5.5 * BLOCK, START_LEVEL_Y - BLOCK),
        std::make_pair(START_LEVEL_X + 2 * BLOCK, START_LEVEL_Y - 5 * BLOCK),
        std::make_pair(START_LEVEL_X + 9 * BLOCK, START_LEVEL_Y - 5 * BLOCK)
    };
    
    std::vector<sf::Color> targetColors = {
        sf::Color(255, 255, 255),
        sf::Color(255, 255, 255), 
        sf::Color(255, 255, 255)
    };
    
    std::vector<std::string> texturePaths = {
        "./assets/textures/milei/kirchner.png",
        "./assets/textures/milei/fernandez.png",
        "./assets/textures/milei/massa.png"
    };
    
    std::vector<SFMLShapeType> targetTypes = {
        SFMLShapeType::CIRCLE,
        SFMLShapeType::TRIANGLE,
        SFMLShapeType::HEXAGON
    };

    l->setTargets(targetSizes, targetPositions, targetColors, texturePaths, targetTypes);

    return l;
}

// Ejemplo de cómo integrar esto en return_level
Level* create_enhanced_level_0() {
    // En lugar de usar el case 0 original, usar esta versión mejorada
    return create_mixed_shapes_level();
}

// Función de prueba
void test_mixed_shapes() {
    Level* nivel = create_mixed_shapes_level();
    
    std::cout << "=== NIVEL CON SHAPES MIXTOS CREADO ===\n";
    std::cout << "Objetos: " << nivel->objects.size() << "\n";
    std::cout << "Targets: " << nivel->targets.size() << "\n";
    std::cout << "Suelo: " << nivel->floor.size() << "\n";
    std::cout << "Tipos de shapes usados:\n";
    std::cout << "• Rectángulos para estructura principal\n";
    std::cout << "• Círculo decorativo dorado\n";
    std::cout << "• Triángulo estructural verde\n";
    std::cout << "• Diamante carmesí especial\n";
    std::cout << "• Hexágono violeta único\n";
    std::cout << "• Targets con formas variadas\n";
    
    delete nivel;
}

/* 
INSTRUCCIONES PARA USAR EN TU PROYECTO:

1. Para usar este nivel mejorado, modifica return_level() en Levels.cpp:

   case 0:
     return create_mixed_shapes_level();

2. O crea un nuevo case para este nivel:

   case 5:  // Nuevo nivel con shapes mixtos
     return create_mixed_shapes_level();

3. El nivel ahora incluye:
   - Rectángulos para estructura principal
   - Círculos decorativos
   - Triángulos estructurales
   - Diamantes especiales
   - Hexágonos únicos
   - Targets con diferentes formas

4. Cada tipo de shape tiene su física apropiada:
   - Círculos usan física circular
   - Otros shapes usan física de caja

5. Mantiene compatibilidad total con el código existente
*/ 