#include "scripts/glGraphics/Vertex.h"
#include "scripts/glGraphics/Texture2D.h"
#include "scripts/glGraphics/Shader.h"
#include "scripts/glGraphics/buffers/VertexArrayObject.h"
#include "scripts/glGraphics/buffers/VertexBufferObject.h"
#include "scripts/glGraphics/buffers/ElementBufferObject.h"

#ifndef MESH_H 
#define MESH_H

#include <vector>

class Mesh {
private:
    VertexArrayObject VAO;
    VertexBufferObject VBO;
    ElementBufferObject EBO;

public:
    std::uint16_t ShaderID;
    std::string name;
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture2D> textures;

    Mesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, std::vector<Texture2D>& textures, std::uint16_t shaderID, const std::string& name = "");

    void Draw(std::vector<Shader>& shaders);
};

#endif // MESH_H
