#include "graphics/VertexBufferObject.h"

#ifndef VAO_H
#define VAO_H

#include <glad/glad.h>

class VertexArrayObject {
public:
    GLuint ID;

    VertexArrayObject();

    void LinkVBO(VertexBufferObject VBO, GLuint layout);

    void Bind();

    void Unbind();

    void Delete();
};

#endif // VAO_H