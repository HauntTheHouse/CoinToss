#pragma once

#include <glad/glad.h>
#include <glm/vec2.hpp>

class Framebuffer
{
public:
    enum class Mode : uint8_t
    {
        RGB,
        DEPTH
    };

    Framebuffer() = default;
    void init(glm::vec2 aSize, Mode aMode);
    void clear();

    void bind() const;
    void unbind() const;

    const GLuint getTexture() const;

private:
    GLuint mFramebufferId{ 0 };
    GLuint mTextureId{ 0 };

    glm::vec2 mSize;
    Mode mMode;
};
