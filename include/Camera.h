#pragma once;

#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glad/glad.h>

struct Projective
{
    glm::mat4 projSpace;

    float mFovy{ 45.0f };
    float mNear{ 0.1f };
    float mFar{ 100.0f };

    void calcProjSpace(GLuint aProgramId) noexcept;
};

struct Camera
{
    glm::mat4 mViewSpace;

    glm::dvec2 mOldPos;

    float mRadius{ 3.0f };
    float mPitch{ 45.0f };
    float mYaw{ 0.0f };

    glm::vec3 mCenter{ 0.0f };
    glm::vec3 mWorldUp{ 0.0f, 1.0f, 0.0f };

    float mRoundMoveSensitivity{ 0.5 };
    float mFlatMoveSensitivity{ 0.005f };
    float mZoomSensitivity{ 0.1f };

    void roundMove(const glm::vec2& aOffset) noexcept;
    void flatMove(const glm::vec2& aOffset) noexcept;
    void zoomMove(float aOffset) noexcept;

    void calcViewSpace(GLuint aProgramId) noexcept;
};
