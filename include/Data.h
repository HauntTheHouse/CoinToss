#pragma once
#include <glad/glad.h>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include "Model.h"

struct Data
{
    GLuint mProgramId{ 0 };

    bool mLeftMousePressed{ false };
    bool mRightMousePressed{ false };

    glm::vec4 mClearColor;

    glm::mat4 mPlatformModelSpace;

    Model mModel;
};
