#include "Slingshot.h"
#include <vector>

namespace {
// --- FUNCIÓN DE AYUDA MOVIDA AQUÍ ---
// Convierte valores de HSV (Tono, Saturación, Valor) a un color RGB.
// Esto permite crear el efecto de arcoíris fácilmente.
sf::Color hsv(float h, float s, float v) {
    int hi = static_cast<int>(std::floor(h / 60.0f)) % 6;
    float f = h / 60.0f - std::floor(h / 60.0f);
    float p = v * (1.0f - s);
    float q = v * (1.0f - f * s);
    float t = v * (1.0f - (1.0f - f) * s);

    sf::Color color;
    switch (hi) {
        case 0: color = sf::Color(v * 255, t * 255, p * 255); break;
        case 1: color = sf::Color(q * 255, v * 255, p * 255); break;
        case 2: color = sf::Color(p * 255, v * 255, t * 255); break;
        case 3: color = sf::Color(p * 255, q * 255, v * 255); break;
        case 4: color = sf::Color(t * 255, p * 255, v * 255); break;
        case 5: color = sf::Color(v * 255, p * 255, q * 255); break;
    }
    return color;
}

// Paleta de colores para el Bosque Ancestral
const sf::Color ANCIENT_WOOD_COLOR(140, 120, 110); // Madera grisácea/marrón
const sf::Color WOOD_GLOW_COLOR(180, 255, 180, 100); // Un suave brillo verde vital
const sf::Color VINE_COLOR(34, 139, 34);          // Verde oscuro para las enredaderas
const sf::Color LEAF_COLOR(80, 200, 120);         // Verde claro para las hojas

} // namespace

Slingshot::Slingshot(sf::Vector2f &pr) {
    leftPost.setSize({POST_WIDTH, POST_HEIGHT});
    rightPost.setSize({POST_WIDTH, POST_HEIGHT});

    leftBand.setPrimitiveType(sf::TriangleStrip);
    rightBand.setPrimitiveType(sf::TriangleStrip);

    leftVine.setPrimitiveType(sf::TriangleStrip);
    rightVine.setPrimitiveType(sf::TriangleStrip);

    this->pos_resortera = pr;

    configurePosts();
    configureBands();
    configureVines();
}

sf::Texture Slingshot::createAncientWoodTexture() {
    sf::Texture texture;
    texture.create(64, 128);
    std::vector<sf::Uint8> pixels(64 * 128 * 4);

    for (int y = 0; y < 128; ++y) {
        for (int x = 0; x < 64; ++x) {
            size_t index = (y * 64 + x) * 4;
            float sine_val = std::sin(static_cast<float>(y) / 20.0f + static_cast<float>(x) / 8.0f);
            int grain_offset = static_cast<int>(sine_val * 10);

            pixels[index] = ANCIENT_WOOD_COLOR.r + grain_offset;
            pixels[index + 1] = ANCIENT_WOOD_COLOR.g + grain_offset;
            pixels[index + 2] = ANCIENT_WOOD_COLOR.b + grain_offset;
            pixels[index + 3] = 255;
        }
    }
    texture.update(pixels.data());
    return texture;
}


void Slingshot::configurePosts() {
    static auto woodTexture = createAncientWoodTexture();
    woodTexture.setRepeated(true);

    leftPost.setTexture(&woodTexture);
    leftPost.setOrigin(POST_WIDTH / 2.f, 0);
    leftPost.setPosition(pos_resortera + sf::Vector2f(-POST_WIDTH * 1.5f, -POST_HEIGHT / 2.f));
    leftPost.setRotation(-10.0f);

    rightPost.setTexture(&woodTexture);
    rightPost.setOrigin(POST_WIDTH / 2.f, 0);
    rightPost.setPosition(pos_resortera + sf::Vector2f(POST_WIDTH * 1.5f, -POST_HEIGHT / 2.f));
    rightPost.setRotation(10.0f);
}

void Slingshot::configureBands() {
    leftBand.resize(10);
    rightBand.resize(10);
}

void Slingshot::configureVines() {
    const int segments = 20;
    leftVine.resize(segments * 2);
    rightVine.resize(segments * 2);

    auto generate_vine = [&](sf::VertexArray& vine, const sf::RectangleShape& post) {
        sf::Transform postTransform = post.getTransform();
        for(int i = 0; i < segments; ++i) {
            float ratio = static_cast<float>(i) / (segments - 1);
            float angle = ratio * 360.0f * 3;
            float x_offset = std::cos(angle * 3.14159f / 180.f) * (POST_WIDTH / 2.f + 5.f);
            float y_pos = ratio * POST_HEIGHT * 0.8f + 30.f;

            sf::Vector2f point(POST_WIDTH / 2.f + x_offset, y_pos);
            sf::Vector2f normal(-std::sin(angle * 3.14159f / 180.f), 0);

            vine[i * 2].position = postTransform.transformPoint(point + normal * 3.f);
            vine[i * 2 + 1].position = postTransform.transformPoint(point - normal * 3.f);

            if(i % 5 == 0) {
                 vine[i*2].color = LEAF_COLOR;
                 vine[i*2+1].color = LEAF_COLOR;
            } else {
                 vine[i*2].color = VINE_COLOR;
                 vine[i*2+1].color = VINE_COLOR;
            }
        }
    };

    generate_vine(leftVine, leftPost);
    generate_vine(rightVine, rightPost);
}

void Slingshot::updateBandGeometry(sf::VertexArray &band,
                                   const sf::Vector2f &start,
                                   const sf::Vector2f &end) {
    const int segments = (band.getVertexCount() / 2) - 1;
    sf::Vector2f sag_direction = end - start;
    float length = std::hypot(sag_direction.x, sag_direction.y);
    sag_direction = (length > 0) ? sag_direction / length : sf::Vector2f();

    for (int i = 0; i <= segments; ++i) {
        float ratio = static_cast<float>(i) / segments;
        sf::Vector2f current_point = start + (end - start) * ratio;

        float sag = std::sin(ratio * 3.14159f) * 20.f;
        sf::Vector2f sag_normal(-sag_direction.y, sag_direction.x);
        current_point += sag_normal * sag;

        sf::Vector2f normal = (i < segments) ? (start + (end-start) * (static_cast<float>(i+1)/segments) - current_point) : (end - current_point);
        float len = std::hypot(normal.x, normal.y);
        if(len > 0) normal /= len;
        normal = sf::Vector2f(-normal.y, normal.x);

        sf::Vector2f offset = normal * BAND_THICKNESS * 0.5f;

        band[i * 2].position = current_point - offset;
        band[i * 2 + 1].position = current_point + offset;
    }
}


void Slingshot::updateBands(const sf::Vector2f &birdPosition, bool isStretched) {
    sf::Transform leftTransform = leftPost.getTransform();
    sf::Transform rightTransform = rightPost.getTransform();
    sf::Vector2f leftStart = leftTransform.transformPoint({POST_WIDTH / 2.f, 60.f});
    sf::Vector2f rightStart = rightTransform.transformPoint({POST_WIDTH / 2.f, 60.f});

    sf::Vector2f birdPos = isStretched ? birdPosition : (leftStart + rightStart) * 0.5f + sf::Vector2f(0, SAG_AMOUNT);

    updateBandGeometry(leftBand, leftStart, birdPos);
    updateBandGeometry(rightBand, rightStart, birdPos);
}

void Slingshot::draw(sf::RenderWindow &window) {
    float time = animationClock.getElapsedTime().asSeconds();

    float wood_pulse = (std::sin(time * 1.5f) + 1.f) / 2.f;
    sf::Color wood_tint = WOOD_GLOW_COLOR;
    wood_tint.a = static_cast<sf::Uint8>(wood_pulse * 40);
    leftPost.setFillColor(sf::Color(255, 255, 255, 255 - wood_tint.a));
    rightPost.setFillColor(sf::Color(255, 255, 255, 255 - wood_tint.a));

    for (size_t i = 0; i < leftBand.getVertexCount(); ++i) {
        float ratio = static_cast<float>(i) / leftBand.getVertexCount();
        float hue = std::fmod((time * 50.f + ratio * 180.f), 360.f);
        sf::Color spectral_color = hsv(hue, 0.7f, 1.0f);
        spectral_color.a = static_cast<sf::Uint8>(150 + std::sin(time * 5.f + ratio * 20.f) * 50);

        leftBand[i].color = spectral_color;
        rightBand[i].color = spectral_color;
    }

    window.draw(leftPost);
    window.draw(rightPost);
    window.draw(leftVine);
    window.draw(rightVine);
    window.draw(leftBand);
    window.draw(rightBand);
}
