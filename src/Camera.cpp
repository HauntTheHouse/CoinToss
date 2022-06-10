#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

#include "System.h"
#include "Utils.h"

void Projective::calcProjSpace(GLuint aProgramId) noexcept
{
    const auto aspect = System::getWindowParameters().mWidth / static_cast<float>(System::getWindowParameters().mHeight);
    mProjSpace = glm::perspective(mFovy, aspect, mNear, mFar);

    glUseProgram(aProgramId);
    Utils::setUniformMat4(aProgramId, "uProjection", mProjSpace);
}

void Camera::roundMove(const glm::vec2& aOffset) noexcept
{
    mYaw -= aOffset.x * mRoundMoveSensitivity;
    mPitch -= aOffset.y * mRoundMoveSensitivity;

    if (mPitch <= 0.0f)
        mPitch = 1.0f;
    else if (mPitch >= 180.0f)
        mPitch = 179.0f;
}

void Camera::flatMove(const glm::vec2& aOffset) noexcept
{
    auto centerViewSpace = mViewSpace * glm::vec4(mCenter, 1.0f);
    centerViewSpace.x -= aOffset.x * mFlatMoveSensitivity;
    centerViewSpace.y += aOffset.y * mFlatMoveSensitivity;
    mCenter = glm::inverse(mViewSpace) * centerViewSpace;
}

void Camera::zoomMove(float aOffset) noexcept
{
    mRadius -= aOffset * mZoomSensitivity;
    const float halfNear = System::getProjective().mNear / 2.0f;
    if (mRadius < halfNear)
        mRadius = halfNear;
}

glm::vec2 Camera::calcOffset() noexcept
{
    double xPos, yPos;
    glfwGetCursorPos(System::getWindowParameters().mWindow, &xPos, &yPos);

    double xOffset = xPos - mOldPos.x;
    double yOffset = yPos - mOldPos.y;

    mOldPos.x = xPos;
    mOldPos.y = yPos;

    return glm::vec2(xOffset, yOffset);
}

void Camera::calcViewSpace(GLuint aProgramId) noexcept
{
    const auto camX = sinf(glm::radians(mPitch)) * sinf(glm::radians(mYaw)) * mRadius + mCenter.x;
    const auto camY = cosf(glm::radians(mPitch)) * mRadius + mCenter.y;
    const auto camZ = sinf(glm::radians(mPitch)) * cosf(glm::radians(mYaw)) * mRadius + mCenter.z;

    mViewSpace = glm::lookAt(glm::vec3(camX, camY, camZ), mCenter, mWorldUp);

    glUseProgram(aProgramId);
    Utils::setUniformMat4(aProgramId, "uView", mViewSpace);
}

