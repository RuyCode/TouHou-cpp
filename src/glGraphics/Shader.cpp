#include "glGraphics/Shader.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <cerrno>
#include <iostream>
#include <unordered_set>

namespace {
    enum class ObjectType {
        VERTEX_SHADER, 
        FRAGMENT_SHADER,
        SHADER_PROGRAM
    };

    void CompileErrors(unsigned int objectID, ObjectType type) {
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

std::string LoadShader(const std::string& filename, std::unordered_set<std::string>& includedFiles) {
    std::cout << "Including shader: " << filename << std::endl;

    if (includedFiles.find(filename) != includedFiles.end()) {
        throw std::runtime_error("Warning: Circular include detected: " + filename);
    }
    includedFiles.insert(filename);

    std::ifstream file(filename);
    if (!file) {
        throw std::runtime_error("Error: Could not open shader file: " + filename);
    }

    std::stringstream buffer;
    std::string line;

    while (std::getline(file, line)) {
        size_t pos = line.find("#pragma include");
        if (pos != std::string::npos) {
            std::string includeFile = line.substr(pos + 17);
            includeFile.erase(0, includeFile.find_first_not_of(" \t\""));
            includeFile.erase(includeFile.find_last_not_of(" \t\"") + 1);

            buffer << LoadShader(includeFile, includedFiles) << "\n";
        } else {
            buffer << line << "\n";
        }
    }

    return buffer.str();
}

std::string LoadShader(const std::string& filename) {
    std::unordered_set<std::string> includedFiles;
    return LoadShader(filename, includedFiles);
}


Shader::Shader(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath) {
    std::string vertexShaderCode = LoadShader(vertexShaderFilePath);
    std::string fragmentShaderCode = LoadShader(fragmentShaderFilePath);

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

    std::cout << "Loaded shader program: " << vertexShaderFilePath << " and " << fragmentShaderFilePath << " with ID of " << ID << std::endl;
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
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::SetFloat(const std::string &name, float value) const {
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::SetVec3(const std::string &name, const glm::vec3& value, unsigned int number) {
    glUniform3fv(glGetUniformLocation(ID, name.c_str()), number, glm::value_ptr(value));
}

void Shader::SetVec4(const std::string &name, const glm::vec4& value, unsigned int number) {
    glUniform4fv(glGetUniformLocation(ID, name.c_str()), number, glm::value_ptr(value));
}

void Shader::SetMat4(const std::string &name, const glm::mat4& value, unsigned int number) {
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), number, GL_FALSE, glm::value_ptr(value));
}

Shader::~Shader() {
    std::cout << "deleted shader" << std::endl;
    Delete();
}
