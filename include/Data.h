#pragma once

#include <vector>
#include <unordered_map>

#include <glad/glad.h>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include "Model.h"

struct Data
{
    GLuint mProgramId{ 0 };

    bool mLeftMousePressed{ false };
    bool mRightMousePressed{ false };

    std::unordered_map<size_t, ModelMeshes> mModelMeshesContainer;
    std::vector<Model> mModels;

    glm::vec4 mClearColor;
};
