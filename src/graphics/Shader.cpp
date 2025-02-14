#include "graphics/Shader.h"

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <cerrno>

#include <iostream>

std::string getFileContents(const char* filePath) {
    std::ifstream file(filePath, std::ios::binary);

    if (!file) {
        throw errno;
    };

    std::string fileContents{};
    
    file.seekg(0, std::ios::end);
    fileContents.resize(file.tellg());

    file.seekg(0, std::ios::beg);
    file.read(&fileContents[0], fileContents.size());

    file.close();

    return fileContents;
}

Shader::Shader(const char* vertexShaderFilePath, const char* fragmentShaderFilePath) {
    std::string vertexShaderCode = getFileContents(vertexShaderFilePath);
    std::string fragmentShaderCode = getFileContents(fragmentShaderFilePath);

    const char* vertexShaderSource = vertexShaderCode.c_str();
    const char* fragmentShaderSource = fragmentShaderCode.c_str();

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    ID = glCreateProgram();
    
    glAttachShader(ID, vertexShader);
    glAttachShader(ID, fragmentShader);

    glLinkProgram(ID);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Shader::Activate() {
    glUseProgram(ID);
}

void Shader::Delete() {
    glDeleteProgram(ID);
}