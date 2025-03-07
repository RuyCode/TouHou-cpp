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

    std::uint16_t width;
    std::uint16_t height;
    
public:
    GLuint ID;

    Texture2D();

    Texture2D(std::uint16_t width, std::uint16_t height);

    Texture2D(std::vector<std::uint8_t> rawData, int index, TextureType type = TextureType::Albedo);

    Texture2D(const std::string& filePath, TextureType type = TextureType::Albedo);

    void BindToUnit(std::uint8_t unitNumber);

    TextureType GetType();

    std::string GetPath();

    glm::vec2 GetSize();

    void LoadFromMemory(const std::vector<std::uint8_t>& rawData);

    void LoadFromFile(const std::string& filePath);

    ~Texture2D();
};

#endif // TEXTURE_H