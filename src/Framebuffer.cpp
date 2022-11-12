#include "Framebuffer.h"

void Framebuffer::init(glm::vec2 aSize, Mode aMode)
{
    if (mFramebufferId != 0)
        return;

    mSize = aSize;
    mMode = aMode;

    glGenTextures(1, &mTextureId);
    glBindTexture(GL_TEXTURE_2D, mTextureId);

    if (mMode == Mode::DEPTH)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, mSize.x, mSize.y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    }
    else if (mMode == Mode::RGB)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, mSize.x, mSize.y, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


    glGenFramebuffers(1, &mFramebufferId);
    glBindFramebuffer(GL_FRAMEBUFFER, mFramebufferId);

    if (mMode == Mode::DEPTH)
    {
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, mTextureId, 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
    }
    else if (mMode == Mode::RGB)
    {
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mTextureId, 0);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::clear()
{
    glDeleteTextures(1, &mTextureId);
    glDeleteFramebuffers(1, &mFramebufferId);
}

void Framebuffer::bind() const
{
    glViewport(0, 0, mSize.x, mSize.y);
    glBindFramebuffer(GL_FRAMEBUFFER, mFramebufferId);

    GLbitfield clearBitmask = GL_DEPTH_BUFFER_BIT;
    if (mMode == Mode::RGB) clearBitmask |= GL_COLOR_BUFFER_BIT;
    glClear(clearBitmask);
}

void Framebuffer::unbind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

const GLuint Framebuffer::getTexture() const
{
    return mTextureId;
}
