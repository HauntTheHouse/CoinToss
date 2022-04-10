#pragma once
#include <glad/glad.h>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

struct Data
{
    GLuint platformVAO{ 0 };
    GLuint platformVBO{ 0 };
    GLuint platformEBO{ 0 };
    GLuint programId{ 0 };

    glm::vec4 clearColor;

    glm::mat4 projSpace;
    glm::mat4 viewSpace;
    glm::mat4 platformModelSpace;
};
