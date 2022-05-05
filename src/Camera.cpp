#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

#include "System.h"
#include "Utils.h"

void Projective::calcProjSpace(GLuint aProgramId) noexcept
{
    const auto aspect = System::getWindowParameters().mWidth / static_cast<float>(System::getWindowParameters().mHeight);
    projSpace = glm::perspective(mFovy, aspect, mNear, mFar);
    Utils::setUniformMat4(aProgramId, "uProjection", projSpace);
}

void Camera::roundMove(const glm::vec2& aOffset) noexcept
{
    mYaw -= aOffset.x;
    mPitch -= aOffset.y;

    if (mPitch <= 0.0f)
        mPitch = 1.0f;
    else if (mPitch >= 180.0f)
        mPitch = 179.0f;
}

void Camera::flatMove(const glm::vec2& aOffset) noexcept
{
    auto centerViewSpace = mViewSpace * glm::vec4(mCenter, 1.0f);
    centerViewSpace.x -= aOffset.x * mMoveSensitivity;
    centerViewSpace.y += aOffset.y * mMoveSensitivity;
    mCenter = glm::inverse(mViewSpace) * centerViewSpace;
}

void Camera::zoomMove(float aOffset) noexcept
{
    mRadius -= aOffset * mZoomSensitivity;
    if (mRadius < 0.15f)
        mRadius = 0.15f;
}

void Camera::calcViewSpace(GLuint aProgramId) noexcept
{
    const auto camX = sinf(glm::radians(mPitch)) * sinf(glm::radians(mYaw)) * mRadius + mCenter.x;
    const auto camY = cosf(glm::radians(mPitch)) * mRadius + mCenter.y;
    const auto camZ = sinf(glm::radians(mPitch)) * cosf(glm::radians(mYaw)) * mRadius + mCenter.z;

    mViewSpace = glm::lookAt(glm::vec3(camX, camY, camZ), mCenter, mWorldUp);
    Utils::setUniformMat4(aProgramId, "uView", mViewSpace);
}
