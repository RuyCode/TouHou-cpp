#ifndef TEXTURE_H
#define TEXTURE_H

#include <SFML/Graphics.hpp>
#include <glad/glad.h>
#include <cstdint>
#include <string>
#include <glm/glm.hpp>

enum class TextureType {
    Albedo,
    Metallic,
    Roughness,
    AmbientOcclusion,
    Height,
    NormalMap
};

class Texture2D {
private:
    sf::Image image;
    TextureType type;
    std::string path;
    
public:
    GLuint ID;

    Texture2D(std::vector<std::uint8_t> rawData, int index, TextureType type);

    Texture2D(const std::string& filePath, TextureType type);

    void BindToUnit(std::uint8_t unitNumber);

    TextureType GetType();

    std::string GetPath();
};

#endif // TEXTURE_H