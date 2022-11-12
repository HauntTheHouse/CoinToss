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
#include "Framebuffer.h"
#include "Plane.h"

struct Data
{
    GLuint mModelsProgramId;

    GLuint mDepthProgramId;
    Framebuffer mDepthFramebuffer;

    GLuint mDepthToColorProgramId;
    Framebuffer mDepthToColorFramebuffer;

    bool mLeftMousePressed{ false };
    bool mRightMousePressed{ false };

    DebugDraw mDebugDraw;

    //std::vector<ModelMeshes> mModelMeshesContainer;
    //std::vector<btCollisionShape> mCylinderShapeContainer;
    //std::vector<MotionState> mMotionStateContainer;
    //std::vector<btRigidBody> mRigidBodyContainer;

    std::vector<glm::mat4> mTransformContainer;

    std::vector<Model> mModels;
    Plane mPlane;

    glm::vec3 mLightColor;
    glm::vec3 mLightDir;
    glm::vec4 mClearColor;
};
