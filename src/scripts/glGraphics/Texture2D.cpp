#include "scripts/glGraphics/Texture2D.h"
#include "scripts/utils/Logger.h"

#include <SFML/Graphics.hpp>
#include <glad/glad.h>
#include <cstdint>
#include <string>
#include <iostream>

Texture2D::Texture2D() : Texture2D(0, 0) {};

Texture2D::Texture2D(std::uint16_t width, std::uint16_t height) 
    : width(width), height(height), type(TextureType::Albedo) {
    glGenTextures(1, &ID);
    glBindTexture(GL_TEXTURE_2D, ID);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    Logger::Instance().Info("created empty texture with width ", width, " and height ", height, ", ID: ", ID);
}

Texture2D::Texture2D(const std::vector<std::uint8_t>& rawData, int index, TextureType type) : type(type) {
    image = sf::Image();

    if (!image.loadFromMemory(rawData.data(), rawData.size() * sizeof(std::uint8_t))) {
        throw std::runtime_error("Error: failed to load texture from memory");
    }

    glGenTextures(1, &ID);
    glBindTexture(GL_TEXTURE_2D, ID);

    width = image.getSize().x;
    height = image.getSize().y;

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.getPixelsPtr());
    glGenerateMipmap(GL_TEXTURE_2D);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    path = "*" + std::to_string(index);

    Logger::Instance().Info("Loaded texture: ", path, ", ID: ", ID);
}

Texture2D::Texture2D(const std::string& filePath, TextureType type) : type(type) {
    this->type = type;
    image = sf::Image();

    if (!image.loadFromFile(filePath)) {
        throw std::runtime_error("Error: failed to load texture from file: " + filePath);
    };

    glGenTextures(1, &ID);
    glBindTexture(GL_TEXTURE_2D, ID);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.getSize().x, image.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.getPixelsPtr());
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    path = filePath;

    Logger::Instance().Info("Loaded texture: ", path, ", ID: ", ID);
}

Texture2D::Texture2D(const Texture2D& other)
    : image(other.image), type(other.type), path(other.path), 
      width(other.width), height(other.height), ID(other.ID) {

    Logger::Instance().Info("Copied texture with width ", width, " and height ", height, ", ID: ", ID);
}

Texture2D::Texture2D(Texture2D&& other) noexcept
    : image(std::move(other.image)), type(std::move(other.type)),
      path(std::move(other.path)), width(other.width), height(other.height),
      ID(other.ID) {
    other.width = 0;
    other.height = 0;
    other.ID = 0;
}

Texture2D& Texture2D::operator=(const Texture2D& other) {
    if (this == &other) {
        return *this;
    }

    image = other.image;
    type = other.type;
    path = other.path;
    width = other.width;
    height = other.height;
    ID = other.ID;

    return *this;
}

Texture2D& Texture2D::operator=(Texture2D&& other) noexcept {
    if (this == &other) {
        return *this;
    }

    image = std::move(other.image);
    type = std::move(other.type);
    path = std::move(other.path);
    width = other.width;
    height = other.height;
    ID = other.ID;

    other.width = 0;
    other.height = 0;
    other.ID = 0;

    return *this;
}

void Texture2D::LoadFromMemory(const std::vector<std::uint8_t>& rawData) {
    if (!image.loadFromMemory(rawData.data(), rawData.size())) {
        Logger::Instance().Error("Error: failed to load texture from memory");
        throw std::runtime_error("Error: failed to load texture from memory");
    }

    glBindTexture(GL_TEXTURE_2D, ID);

    if (image.getSize().x != width || image.getSize().y != height) {
        width = image.getSize().x;
        height = image.getSize().y;
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.getPixelsPtr());
    } else {
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, image.getPixelsPtr());
    }

    glGenerateMipmap(GL_TEXTURE_2D);
}

void Texture2D::LoadFromFile(const std::string& filePath) {
    if (!image.loadFromFile(filePath)) {
        Logger::Instance().Error("Error: failed to load texture from file: ", filePath);
        throw std::runtime_error("Error: failed to load texture from file: " + filePath);
    }

    glBindTexture(GL_TEXTURE_2D, ID);

    if (image.getSize().x != width || image.getSize().y != height) {
        width = image.getSize().x;
        height = image.getSize().y;
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.getPixelsPtr());
    } else {
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, image.getPixelsPtr());
    }

    glGenerateMipmap(GL_TEXTURE_2D);
    path = filePath;
}

void Texture2D::BindToUnit(std::uint8_t unitNumber) {
    glActiveTexture(GL_TEXTURE0 + unitNumber);
    glBindTexture(GL_TEXTURE_2D, ID);
}

TextureType Texture2D::GetType() {
    return type;
}

std::string Texture2D::GetPath() {
    return path;
}

glm::vec2 Texture2D::GetSize() {
    return glm::vec2(width, height);
}

Texture2D::~Texture2D() {
    Logger::Instance().Info("deleted texture ", ID);
}
