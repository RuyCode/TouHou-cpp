#include "glGraphics/Shader.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <cerrno>

#include <iostream>

namespace {
    enum class ObjectType {
        VERTEX_SHADER, 
        FRAGMENT_SHADER,
        SHADER_PROGRAM
    };

    void CompileErrors(uint objectID, ObjectType type) {
        int success;
        char infoLog[1024];

        switch (type){
            case ObjectType::VERTEX_SHADER: {
                glGetShaderiv(objectID, GL_COMPILE_STATUS, &success);

                if (!success) {
                    glGetShaderInfoLog(objectID, 1024, NULL, infoLog);
                    std::cerr << infoLog << std::endl;
                    throw std::runtime_error("Error: vertex shader compilation failed");
                }

                break;
            }

            case ObjectType::FRAGMENT_SHADER: {
                glGetShaderiv(objectID, GL_COMPILE_STATUS, &success);

                if (!success) {
                    glGetShaderInfoLog(objectID, 1024, NULL, infoLog);
                    std::cerr << infoLog << std::endl;
                    throw std::runtime_error("Error: fragment shader compilation failed");
                }

                break;
            }

            case ObjectType::SHADER_PROGRAM: {
                glGetProgramiv(objectID, GL_LINK_STATUS, &success);

                if (!success) {
                    glGetProgramInfoLog(objectID, 1024, NULL, infoLog);
                    throw std::runtime_error("Error: failed to link shaders");
                }
            }
        }
    }
}

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

    CompileErrors(vertexShader, ObjectType::VERTEX_SHADER);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    CompileErrors(fragmentShader, ObjectType::FRAGMENT_SHADER);

    ID = glCreateProgram();
    
    glAttachShader(ID, vertexShader);
    glAttachShader(ID, fragmentShader);

    glLinkProgram(ID);

    CompileErrors(ID, ObjectType::SHADER_PROGRAM);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Shader::Activate() {
    glUseProgram(ID);
}

void Shader::Delete() {
    glDeleteProgram(ID);
}

void Shader::SetBool(const std::string &name, bool value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), static_cast<int>(value));
}   

void Shader::SetInt(const std::string &name, int value) const {
    std::cout << name << ": " << glGetUniformLocation(ID, name.c_str()) << std::endl;
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::SetFloat(const std::string &name, float value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::SetVec3(const std::string &name, const glm::vec3& value) {
    glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(value));
}

void Shader::SetMat4(const std::string &name, const glm::mat4& value) {
    glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(value));
}
