#include "MotionState.h"

#include <glm/gtc/quaternion.hpp>

MotionState::MotionState(glm::mat4& aTransform)
    : mTransform(aTransform)
{
}

void MotionState::getWorldTransform(btTransform& worldTrans) const
{
    const auto& pos = mTransform[3];
    const auto rot = glm::quat_cast(mTransform);

    worldTrans.setOrigin(btVector3(pos.x, pos.y, pos.z));
    worldTrans.setRotation(btQuaternion(rot.x, rot.y, rot.z, rot.w));
}

void MotionState::setWorldTransform(const btTransform& worldTrans)
{
    const auto& pos = worldTrans.getOrigin();
    const auto rot = worldTrans.getRotation();
    const auto rotAxis = rot.getAxis();

    mTransform = glm::mat4(1.0f);
    mTransform = glm::translate(mTransform, glm::vec3(pos.x(), pos.y(), pos.z()));
    mTransform = glm::rotate(mTransform, rot.getAngle(), glm::vec3(rotAxis.x(), rotAxis.y(), rotAxis.z()));
}
