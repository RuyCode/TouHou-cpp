#include "glGraphics/Texture2D.h"
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

    path = filePath;
    ID = texture.getNativeHandle();
}

Texture2D::Texture2D(const std::string& filePath, TextureType type) {
    this->type = type;
    texture = sf::Texture();

    if (!texture.loadFromFile(filePath)) {
        throw std::runtime_error("Error: failed to load texture from file: " + filePath);
    };

    if (!texture.generateMipmap()) {
        throw std::runtime_error("Error: failed to generate mipmap for texture: " + filePath);
    };

    path = filePath;
    ID = texture.getNativeHandle();
}

void Texture2D::BindToUnit(std::uint8_t unitNumber) {
    if (unitNumber >= 16) {
        throw std::runtime_error("Error: invalid unit number (must be in range [0, 15])");
    }

    glActiveTexture(GL_TEXTURE0 + unitNumber);
    glBindTexture(GL_TEXTURE_2D, ID);
}

TextureType Texture2D::GetType() {
    return type;
}

std::string Texture2D::GetPath() {
    return path;
}