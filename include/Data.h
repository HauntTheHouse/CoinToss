#pragma once

#include <vector>
#include <unordered_map>

#include <glad/glad.h>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <btBulletCollisionCommon.h>

#include "Model.h"
#include "MotionState.h"
#include "DebugDraw.h"

struct Data
{
    GLuint mModelsProgramId;
    GLuint mDebugDrawProgramId;

    bool mLeftMousePressed{ false };
    bool mRightMousePressed{ false };

    DebugDraw mDebugDraw;

    std::unordered_map<size_t, ModelMeshes> mModelMeshesContainer;

    std::unordered_map<size_t, btCylinderShape> mCylinderShapesContainer;
    std::unordered_map<size_t, MotionState> mMotionStatesContainer;
    std::unordered_map<size_t, btRigidBody> mRigidBodiesContainer;

    std::vector<Model> mModels;

    glm::vec4 mClearColor;
};
