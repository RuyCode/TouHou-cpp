#include "glGraphics/Texture2D.h"

#ifndef FBO_H
#define FBO_H

class FrameBufferObject {
private:
    Texture2D frame;
public:
    GLuint ID;

    FrameBufferObject(std::uint16_t width, std::uint16_t height);

    Texture2D& GetFrame();

    void Bind();

    void BindRead();

    void BindDraw();

    void Unbind();

    void UnbindRead();

    void UnbindDraw();

    void Delete();
};

#endif // FBO_H