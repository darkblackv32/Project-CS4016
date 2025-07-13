# SFML Shapes System Test

This test suite validates the functionality of the SFML shapes system implementation.

## Overview

The test covers the following areas:

1. **Shape Factory Methods** - Tests creation of different shape types (Rectangle, Circle, Triangle, Diamond, Hexagon, Custom Polygon)
2. **Level Objects** - Tests setting objects with different shapes and physics integration
3. **Level Targets** - Tests setting targets with different shapes
4. **Level Floor** - Tests setting floor elements with different shapes
5. **Mixed Shapes** - Tests using all shape types together
6. **Level Initialization** - Tests basic level setup and properties
7. **Shape Type Validation** - Tests all enum values work correctly
8. **Rendering** - Tests that rendering doesn't crash
9. **Physics Integration** - Tests physics bodies are created correctly
10. **Memory Management** - Tests smart pointer cleanup

## How to Run

### Compile and Run Test:
```bash
make test
```

### Compile Only (without running):
```bash
make test-compile
```

### Run Test Manually:
```bash
./test_shapes
```

### Clean Build Files:
```bash
make clean
```

## Test Output

The test will output a series of checkmarks (✓) for passed tests and X marks (✗) for failed tests, organized by test category. Each test validates specific functionality of the shapes system.

## Test Features

- **Comprehensive Coverage**: Tests all major functionality
- **Shape Type Coverage**: Tests all 6 supported shape types
- **Backward Compatibility**: Ensures existing code still works
- **Physics Integration**: Validates Box2D integration
- **Memory Safety**: Tests smart pointer usage
- **Error Handling**: Catches exceptions and reports them
- **Clear Output**: Easy-to-read test results

## Dependencies

The test requires the same dependencies as the main game:
- SFML (Graphics, Window, System, Audio)
- Box2D Physics Library
- C++17 Standard

## Notes

- The test creates a temporary SFML window for rendering tests
- All tests should pass for the system to be considered working correctly
- The test validates both new functionality and backward compatibility 