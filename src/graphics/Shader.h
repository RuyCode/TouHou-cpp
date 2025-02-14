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
};

#endif // SHADER_H