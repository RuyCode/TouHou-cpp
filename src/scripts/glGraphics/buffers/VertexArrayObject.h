#include "scripts/glGraphics/buffers/VertexBufferObject.h"

#ifndef VAO_H
#define VAO_H

#include <glad/glad.h>

class VertexArrayObject {
public:
    GLuint ID;

    VertexArrayObject();

    void LinkAttrib(VertexBufferObject VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset);

    void Bind();

    void Unbind();

    void Delete();
};

#endif // VAO_H
