#pragma once
#include <glad/glad.h>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include "Model.h"

struct Data
{
    GLuint programId{ 0 };

    glm::vec4 clearColor;

    glm::mat4 projSpace;
    glm::mat4 viewSpace;
    glm::mat4 platformModelSpace;

    Model model;
};
