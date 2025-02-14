#include "graphics/VertexBufferObject.h"
#include "graphics/VertexArrayObject.h"

#include <glad/glad.h>

VertexArrayObject::VertexArrayObject() {
    glGenVertexArrays(1, &ID);
}

void VertexArrayObject::LinkVBO(VertexBufferObject VBO, GLuint layout) {
    VBO.Bind();

    glVertexAttribPointer(layout, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(layout);

    VBO.Unbind();
}

void VertexArrayObject::Bind() {
    glBindVertexArray(ID);
}

void VertexArrayObject::Unbind() {
    glBindVertexArray(0);
}

void VertexArrayObject::Delete() {
    glDeleteVertexArrays(1, &ID);
}