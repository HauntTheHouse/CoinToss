#include "MotionState.h"

#include "System.h"
#include <glm/gtc/quaternion.hpp>

MotionState::MotionState(size_t aModelId)
    : mModelId(aModelId)
{
}

void MotionState::getWorldTransform(btTransform& worldTrans) const
{
    const auto& transfrom = System::getData().mTransformContainer[mModelId];
    const auto& pos = transfrom[3];
    const auto rot = glm::quat_cast(transfrom);

    worldTrans.setOrigin(btVector3(pos.x, pos.y, pos.z));
    worldTrans.setRotation(btQuaternion(rot.x, rot.y, rot.z, rot.w));
}

void MotionState::setWorldTransform(const btTransform& worldTrans)
{
    const auto& pos = worldTrans.getOrigin();
    const auto rot = worldTrans.getRotation();
    const auto rotAxis = rot.getAxis();

    auto& transfrom = System::getData().mTransformContainer[mModelId];
    transfrom = glm::mat4(1.0f);
    transfrom = glm::translate(transfrom, glm::vec3(pos.x(), pos.y(), pos.z()));
    transfrom = glm::rotate(transfrom, rot.getAngle(), glm::vec3(rotAxis.x(), rotAxis.y(), rotAxis.z()));
}
