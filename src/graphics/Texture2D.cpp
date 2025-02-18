#include "graphics/Texture2D.h"
#include <SFML/Graphics.hpp>
#include <glad/glad.h>
#include <cstdint>
#include <string>

Texture2D::Texture2D(const std::string& filePath) {
    texture = sf::Texture();

    if (!texture.loadFromFile(filePath)) {
        throw std::runtime_error("Error: failed to load texture from file: " + filePath);
    };

    if (!texture.generateMipmap()) {
        throw std::runtime_error("Error: failed to generate mipmap for texture: " + filePath);
    };

    ID = texture.getNativeHandle();
}