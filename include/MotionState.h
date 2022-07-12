#pragma once

#include <LinearMath/btMotionState.h>
#include <glm/mat4x4.hpp>

class MotionState : public btMotionState
{
public:
    MotionState() = default;
    MotionState(size_t aModelId);

    void getWorldTransform(btTransform& worldTrans) const;
    void setWorldTransform(const btTransform& worldTrans);

private:
    size_t mModelId;
};
