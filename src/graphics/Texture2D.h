#ifndef TEXTURE_H
#define TEXTURE_H

#include <SFML/Graphics.hpp>
#include <glad/glad.h>
#include <cstdint>
#include <string>
#include <glm/glm.hpp>

class Texture2D {
private:
    sf::Texture texture;
    
public:
    GLuint ID;

    Texture2D(const std::string& filePath);

    void BindToUnit(std::uint8_t unitNumber) {
        if (unitNumber >= 16) {
            throw std::runtime_error("Error: invalid unit number (must be in range [0, 15])");
        }

        glActiveTexture(GL_TEXTURE0 + unitNumber);
        glBindTexture(GL_TEXTURE_2D, ID);
    }
};

#endif // TEXTURE_H