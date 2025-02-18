#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <cerrno>

class Shader {
public:
    GLuint ID;
    
    Shader(const char* vertexShaderFilePath, const char* fragmenShadertFilePath);

    void Activate();

    void Delete();

    void SetBool(const std::string &name, bool value) const;

    void SetInt(const std::string &name, int value) const;

    void SetFloat(const std::string &name, float value) const;
};

#endif // SHADER_H