#include "glGraphics/FrameBufferObject.h"

FrameBufferObject::FrameBufferObject(std::uint16_t width, std::uint16_t height) : frame(Texture2D(width, height)) {
    glGenFramebuffers(1, &ID);
    Bind();

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, frame.ID, 0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        throw std::runtime_error("Error: Framebuffer is not complete!");
    }

    Unbind();
}

Texture2D& FrameBufferObject::GetFrame() {
    return frame;
}

void FrameBufferObject::Bind() {
    glBindFramebuffer(GL_FRAMEBUFFER, ID);
}

void FrameBufferObject::BindRead() {
    glBindFramebuffer(GL_READ_FRAMEBUFFER, ID);
}

void FrameBufferObject::BindDraw() {
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, ID);
}

void FrameBufferObject::Unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBufferObject::UnbindRead() {
    glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
}

void FrameBufferObject::UnbindDraw() {
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

void FrameBufferObject::Delete() {
    glDeleteFramebuffers(1, &ID);
}