#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
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

    void SetVec3(const std::string &name, const glm::vec3& value);

    void SetVec4(const std::string &name, const glm::vec4& value);

    void SetMat4(const std::string &name, const glm::mat4& value);
};

#endif // SHADER_H