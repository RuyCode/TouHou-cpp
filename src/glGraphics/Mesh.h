#include "glGraphics/Vertex.h"
#include "glGraphics/Texture2D.h"
#include "glGraphics/Shader.h"
#include "glGraphics/VertexArrayObject.h"
#include "glGraphics/VertexBufferObject.h"
#include "glGraphics/ElementBufferObject.h"

#ifndef MESH_H 
#define MESH_H

#include <vector>

class Mesh {
private:
    VertexArrayObject VAO;
    VertexBufferObject VBO;
    ElementBufferObject EBO;

public:
    std::string name;
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture2D> textures;

    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture2D> textures, std::string name = "");

    void Draw(Shader &shader);
};

#endif // MESH_H