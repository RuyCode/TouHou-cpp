#ifndef EBO_H
#define EBO_H

#include <glad/glad.h>

class ElementBufferObject {
public:
    GLuint ID;

    ElementBufferObject(GLuint* indices, GLsizeiptr size);

    void Bind();

    void Unbind();

    void Delete();
};

#endif // EBO_H