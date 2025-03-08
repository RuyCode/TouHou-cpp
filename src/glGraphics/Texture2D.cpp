#include "glGraphics/Texture2D.h"

#include <SFML/Graphics.hpp>
#include <glad/glad.h>
#include <cstdint>
#include <string>
#include <iostream>

Texture2D::Texture2D() : Texture2D(0, 0) {};

Texture2D::Texture2D(std::uint16_t width, std::uint16_t height) : width(width), height(height) {
    this->type = TextureType::Albedo;

    glGenTextures(1, &ID);
    glBindTexture(GL_TEXTURE_2D, ID);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    std::cout << "created empty texture with width " << width << " and height " << height << ", ID: " << ID << std::endl;
}

Texture2D::Texture2D(std::vector<std::uint8_t> rawData, int index, TextureType type) {
    this->type = type;
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

    std::cout << "loaded texture: " << path << ", ID: " << ID << std::endl;
}

Texture2D::Texture2D(const std::string& filePath, TextureType type) {
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

    std::cout << "loaded texture: " << path << ", ID: " << ID << std::endl;   
}

void Texture2D::LoadFromMemory(const std::vector<std::uint8_t>& rawData) {
    if (!image.loadFromMemory(rawData.data(), rawData.size())) {
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
    std::cout << "deleted texture " << ID << std::endl;
}