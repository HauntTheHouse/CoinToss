#pragma once

#include <LinearMath/btMotionState.h>
#include <glm/mat4x4.hpp>

class MotionState : public btMotionState
{
public:
    MotionState() = default;
    MotionState(glm::mat4& aTransform);

    void getWorldTransform(btTransform& worldTrans) const;
    void setWorldTransform(const btTransform& worldTrans);

private:
    glm::mat4& mTransform;
};
