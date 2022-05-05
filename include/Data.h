#pragma once

#include <vector>

#include <glad/glad.h>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include "Model.h"

struct Data
{
    GLuint mProgramId{ 0 };

    bool mLeftMousePressed{ false };
    bool mRightMousePressed{ false };

    std::vector<Model> mModels;

    glm::vec4 mClearColor;
};
