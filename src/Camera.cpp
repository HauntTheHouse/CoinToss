#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

#include "System.h"
#include "Shader.h"

void Projective::calcProjSpace() noexcept
{
    const auto aspect = System::getWindowParameters().mWidth / static_cast<float>(System::getWindowParameters().mHeight);
    mProjSpace = glm::perspective(mFovy, aspect, mNear, mFar);
}

void Camera::roundMove(glm::vec2 aOffset) noexcept
{
    mYaw -= aOffset.x * mRoundMoveSensitivity;
    mPitch -= aOffset.y * mRoundMoveSensitivity;

    if (mPitch <= 0.0f)
        mPitch = 1.0f;
    else if (mPitch >= 180.0f)
        mPitch = 179.0f;
}

void Camera::flatMove(glm::vec2 aOffset) noexcept
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

void Camera::calcViewSpace() noexcept
{
    mCamPosition.x = sinf(glm::radians(mPitch)) * sinf(glm::radians(mYaw)) * mRadius + mCenter.x;
    mCamPosition.y = cosf(glm::radians(mPitch)) * mRadius + mCenter.y;
    mCamPosition.z = sinf(glm::radians(mPitch)) * cosf(glm::radians(mYaw)) * mRadius + mCenter.z;

    mViewSpace = glm::lookAt(mCamPosition, mCenter, mWorldUp);
}

void Camera::updateCamera()
{
    calcViewSpace();

    Shader::setActiveProgramId(System::getData().mModelsProgramId);
    Shader::setUniformMat4("uView", mViewSpace);
    Shader::setUniformVec3("uCameraPosition", mCamPosition);
}
