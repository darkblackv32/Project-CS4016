#include "Levels.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cassert>
#include <memory>

// Test utility functions
void assert_test(bool condition, const std::string& test_name) {
    if (condition) {
        std::cout << "✓ " << test_name << " PASSED" << std::endl;
    } else {
        std::cout << "✗ " << test_name << " FAILED" << std::endl;
    }
}

void print_test_header(const std::string& test_name) {
    std::cout << "\n=== " << test_name << " ===" << std::endl;
}

// Test 1: Shape Factory Methods
void test_shape_factory() {
    print_test_header("SHAPE FACTORY TESTS");
    
    Level level;
    sf::Vector2f size(50.0f, 50.0f);
    sf::Vector2f position(100.0f, 100.0f);
    sf::Color color = sf::Color::Red;
    
    // Test Rectangle creation
    auto rectangle = level.createSFMLShape(SFMLShapeType::RECTANGLE, size, position, color);
    assert_test(rectangle != nullptr, "Rectangle creation");
    assert_test(rectangle->getPosition() == position, "Rectangle position");
    
    // Test Circle creation
    auto circle = level.createSFMLShape(SFMLShapeType::CIRCLE, size, position, color);
    assert_test(circle != nullptr, "Circle creation");
    assert_test(circle->getPosition() == position, "Circle position");
    
    // Test Triangle creation
    auto triangle = level.createSFMLShape(SFMLShapeType::TRIANGLE, size, position, color);
    assert_test(triangle != nullptr, "Triangle creation");
    assert_test(triangle->getPosition() == position, "Triangle position");
    
    // Test Diamond creation
    auto diamond = level.createSFMLShape(SFMLShapeType::DIAMOND, size, position, color);
    assert_test(diamond != nullptr, "Diamond creation");
    assert_test(diamond->getPosition() == position, "Diamond position");
    
    // Test Hexagon creation
    auto hexagon = level.createSFMLShape(SFMLShapeType::HEXAGON, size, position, color);
    assert_test(hexagon != nullptr, "Hexagon creation");
    assert_test(hexagon->getPosition() == position, "Hexagon position");
    
    // Test Pentagon creation
    auto pentagon = level.createSFMLShape(SFMLShapeType::PENTAGON, size, position, color);
    assert_test(pentagon != nullptr, "Pentagon creation");
    assert_test(pentagon->getPosition() == position, "Pentagon position");
    
    // Test specific shape creation methods
    auto triangleSpecific = level.createTriangle(size, position);
    assert_test(triangleSpecific != nullptr, "Triangle specific creation");
    
    auto diamondSpecific = level.createDiamond(size, position);
    assert_test(diamondSpecific != nullptr, "Diamond specific creation");
    
    auto hexagonSpecific = level.createHexagon(size, position);
    assert_test(hexagonSpecific != nullptr, "Hexagon specific creation");
    
    auto pentagonSpecific = level.createPentagon(size, position);
    assert_test(pentagonSpecific != nullptr, "Pentagon specific creation");
    
    // Test custom polygon creation
    std::vector<sf::Vector2f> vertices = {
        sf::Vector2f(0, 0), sf::Vector2f(30, 0), sf::Vector2f(15, 20)
    };
    auto customPolygon = level.createCustomPolygon(vertices, position);
    assert_test(customPolygon != nullptr, "Custom polygon creation");
    assert_test(customPolygon->getPosition() == position, "Custom polygon position");
}

// Test 2: Level Objects Setting
void test_level_objects() {
    print_test_header("LEVEL OBJECTS TESTS");
    
    Level level;
    
    // Test setting objects with different shapes
    std::vector<sf::Vector2f> sizes = {
        sf::Vector2f(40, 40), sf::Vector2f(30, 30), sf::Vector2f(50, 50)
    };
    std::vector<std::pair<float, float>> positions = {
        {100, 100}, {200, 100}, {300, 100}
    };
    std::vector<sf::Color> colors = {
        sf::Color::Red, sf::Color::Blue, sf::Color::Green
    };
    std::vector<SFMLShapeType> shapeTypes = {
        SFMLShapeType::RECTANGLE, SFMLShapeType::CIRCLE, SFMLShapeType::TRIANGLE
    };
    
    level.setObjects(sizes, positions, colors, shapeTypes);
    assert_test(level.objects.size() == 3, "Objects count");
    assert_test(level.m_bodies.size() == 3, "Physics bodies count");
    
    // Test backward compatibility (no shape types specified)
    Level level2;
    level2.setObjects(sizes, positions, colors);
    assert_test(level2.objects.size() == 3, "Backward compatibility - objects count");
    assert_test(level2.m_bodies.size() == 3, "Backward compatibility - physics bodies count");
}

// Test 3: Level Targets Setting
void test_level_targets() {
    print_test_header("LEVEL TARGETS TESTS");
    
    Level level;
    
    std::vector<sf::Vector2f> sizes = {sf::Vector2f(40, 40), sf::Vector2f(30, 30)};
    std::vector<std::pair<float, float>> positions = {{150, 150}, {250, 150}};
    std::vector<sf::Color> colors = {sf::Color::Yellow, sf::Color::Magenta};
    std::vector<std::string> texturePaths = {"", ""}; // Empty paths for test
    std::vector<SFMLShapeType> shapeTypes = {SFMLShapeType::DIAMOND, SFMLShapeType::HEXAGON};
    
    level.setTargets(sizes, positions, colors, texturePaths, shapeTypes);
    assert_test(level.targets.size() == 2, "Targets count");
    assert_test(level.m_targets.size() == 2, "Target physics bodies count");
}

// Test 4: Level Floor Setting
void test_level_floor() {
    print_test_header("LEVEL FLOOR TESTS");
    
    Level level;
    
    std::vector<sf::Vector2f> sizes = {sf::Vector2f(200, 20), sf::Vector2f(100, 20)};
    std::vector<std::pair<float, float>> positions = {{400, 500}, {600, 500}};
    std::vector<sf::Color> colors = {sf::Color::Blue, sf::Color::Green};
    std::vector<SFMLShapeType> shapeTypes = {SFMLShapeType::RECTANGLE, SFMLShapeType::RECTANGLE};
    
    level.setFloor(sizes, positions, colors, shapeTypes);
    assert_test(level.floor.size() == 2, "Floor count");
    assert_test(level.m_static.size() == 2, "Static physics bodies count");
}

// Test 5: Mixed Shape Types
void test_mixed_shapes() {
    print_test_header("MIXED SHAPES TESTS");
    
    Level level;
    
    // Create a mix of all shape types
    std::vector<sf::Vector2f> sizes = {
        sf::Vector2f(40, 40), sf::Vector2f(30, 30), sf::Vector2f(50, 50),
        sf::Vector2f(35, 35), sf::Vector2f(45, 45), sf::Vector2f(55, 55),
        sf::Vector2f(42, 42)
    };
    std::vector<std::pair<float, float>> positions = {
        {100, 100}, {200, 100}, {300, 100},
        {400, 100}, {500, 100}, {600, 100},
        {700, 100}
    };
    std::vector<sf::Color> colors = {
        sf::Color::Red, sf::Color::Blue, sf::Color::Green,
        sf::Color::Yellow, sf::Color::Magenta, sf::Color::Cyan,
        sf::Color::White
    };
    std::vector<SFMLShapeType> shapeTypes = {
        SFMLShapeType::RECTANGLE, SFMLShapeType::CIRCLE, SFMLShapeType::TRIANGLE,
        SFMLShapeType::DIAMOND, SFMLShapeType::HEXAGON, SFMLShapeType::PENTAGON,
        SFMLShapeType::CUSTOM_POLYGON
    };
    
    level.setObjects(sizes, positions, colors, shapeTypes);
    assert_test(level.objects.size() == 7, "Mixed shapes count");
    assert_test(level.m_bodies.size() == 7, "Mixed shapes physics bodies count");
}

// Test 6: Level Initialization and Properties
void test_level_initialization() {
    print_test_header("LEVEL INITIALIZATION TESTS");
    
    Level level;
    
    // Test initial state
    assert_test(level.objects.empty(), "Initial objects empty");
    assert_test(level.targets.empty(), "Initial targets empty");
    assert_test(level.floor.empty(), "Initial floor empty");
    assert_test(level.m_bodies.empty(), "Initial bodies empty");
    assert_test(level.m_targets.empty(), "Initial target bodies empty");
    assert_test(level.m_static.empty(), "Initial static bodies empty");
    
    // Test setters
    level.setStarts(100.0f, 200.0f);
    assert_test(level.START_LEVEL_X == 100.0f, "Start X position");
    assert_test(level.START_LEVEL_Y == 200.0f, "Start Y position");
    
    level.setBounds(800.0f, 600.0f);
    assert_test(level.x_bound == 800, "X bound");
    assert_test(level.y_bound == 600, "Y bound");
    
    level.setNLevel(5);
    assert_test(level.n_level == 5, "Level number");
}

// Test 7: Shape Type Validation
void test_shape_type_validation() {
    print_test_header("SHAPE TYPE VALIDATION TESTS");
    
    Level level;
    sf::Vector2f size(50.0f, 50.0f);
    sf::Vector2f position(100.0f, 100.0f);
    sf::Color color = sf::Color::Red;
    
    // Test all enum values
    auto rectangle = level.createSFMLShape(SFMLShapeType::RECTANGLE, size, position, color);
    assert_test(rectangle != nullptr, "Rectangle enum validation");
    
    auto circle = level.createSFMLShape(SFMLShapeType::CIRCLE, size, position, color);
    assert_test(circle != nullptr, "Circle enum validation");
    
    auto triangle = level.createSFMLShape(SFMLShapeType::TRIANGLE, size, position, color);
    assert_test(triangle != nullptr, "Triangle enum validation");
    
    auto diamond = level.createSFMLShape(SFMLShapeType::DIAMOND, size, position, color);
    assert_test(diamond != nullptr, "Diamond enum validation");
    
    auto hexagon = level.createSFMLShape(SFMLShapeType::HEXAGON, size, position, color);
    assert_test(hexagon != nullptr, "Hexagon enum validation");
    
    auto pentagon = level.createSFMLShape(SFMLShapeType::PENTAGON, size, position, color);
    assert_test(pentagon != nullptr, "Pentagon enum validation");
    
    auto customPolygon = level.createSFMLShape(SFMLShapeType::CUSTOM_POLYGON, size, position, color);
    assert_test(customPolygon != nullptr, "Custom polygon enum validation");
}

// Test 8: Rendering Test (Basic)
void test_rendering() {
    print_test_header("RENDERING TESTS");
    
    Level level;
    
    // Add some shapes
    std::vector<sf::Vector2f> sizes = {sf::Vector2f(40, 40), sf::Vector2f(30, 30)};
    std::vector<std::pair<float, float>> positions = {{100, 100}, {200, 100}};
    std::vector<sf::Color> colors = {sf::Color::Red, sf::Color::Blue};
    std::vector<SFMLShapeType> shapeTypes = {SFMLShapeType::RECTANGLE, SFMLShapeType::CIRCLE};
    
    level.setObjects(sizes, positions, colors, shapeTypes);
    
    // Create a window for rendering test
    sf::RenderWindow window(sf::VideoMode(800, 600), "Shape Test", sf::Style::Close);
    
    // Test that rendering doesn't crash
    try {
        level.render(window);
        assert_test(true, "Rendering execution");
    } catch (const std::exception& e) {
        assert_test(false, "Rendering execution - Exception: " + std::string(e.what()));
    }
    
    window.close();
}

// Test 9: Physics Integration Test
void test_physics_integration() {
    print_test_header("PHYSICS INTEGRATION TESTS");
    
    Level level;
    
    // Add objects with physics
    std::vector<sf::Vector2f> sizes = {sf::Vector2f(40, 40), sf::Vector2f(30, 30)};
    std::vector<std::pair<float, float>> positions = {{100, 100}, {200, 100}};
    std::vector<sf::Color> colors = {sf::Color::Red, sf::Color::Blue};
    std::vector<SFMLShapeType> shapeTypes = {SFMLShapeType::RECTANGLE, SFMLShapeType::CIRCLE};
    
    level.setObjects(sizes, positions, colors, shapeTypes);
    
    // Test physics bodies creation
    assert_test(level.m_bodies.size() == 2, "Physics bodies created");
    assert_test(level.m_bodies[0] != nullptr, "First physics body valid");
    assert_test(level.m_bodies[1] != nullptr, "Second physics body valid");
    
    // Test physics step
    try {
        level.run(1.0f/60.0f); // 60 FPS delta time
        assert_test(true, "Physics step execution");
    } catch (const std::exception& e) {
        assert_test(false, "Physics step execution - Exception: " + std::string(e.what()));
    }
}

// Test 10: Memory Management Test
void test_memory_management() {
    print_test_header("MEMORY MANAGEMENT TESTS");
    
    // Test smart pointer management
    {
        Level level;
        std::vector<sf::Vector2f> sizes = {sf::Vector2f(40, 40), sf::Vector2f(30, 30)};
        std::vector<std::pair<float, float>> positions = {{100, 100}, {200, 100}};
        std::vector<sf::Color> colors = {sf::Color::Red, sf::Color::Blue};
        std::vector<SFMLShapeType> shapeTypes = {SFMLShapeType::RECTANGLE, SFMLShapeType::CIRCLE};
        
        level.setObjects(sizes, positions, colors, shapeTypes);
        level.setTargets(sizes, positions, colors, {"", ""}, shapeTypes);
        level.setFloor(sizes, positions, colors, shapeTypes);
        
        assert_test(level.objects.size() == 2, "Objects memory allocation");
        assert_test(level.targets.size() == 2, "Targets memory allocation");
        assert_test(level.floor.size() == 2, "Floor memory allocation");
    } // Level goes out of scope, automatic cleanup should happen
    
    assert_test(true, "Memory cleanup (no crash)");
}

// Main test runner
int main() {
    std::cout << "Starting SFML Shapes System Tests..." << std::endl;
    std::cout << "=====================================" << std::endl;
    
    try {
        test_shape_factory();
        test_level_objects();
        test_level_targets();
        test_level_floor();
        test_mixed_shapes();
        test_level_initialization();
        test_shape_type_validation();
        test_rendering();
        test_physics_integration();
        test_memory_management();
        
        std::cout << "\n=====================================" << std::endl;
        std::cout << "All tests completed!" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Test suite failed with exception: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
} 