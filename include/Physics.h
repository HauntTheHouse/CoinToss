#pragma once

#include <btBulletDynamicsCommon.h>

class Physics
{
public:
    Physics()
        : mConstraintSolver()
        , mCollisionConfiguration()
        , mDispatcher(&mCollisionConfiguration)
        , mBroadphase()
        , mDynamicWorld(&mDispatcher, &mBroadphase, &mConstraintSolver, &mCollisionConfiguration)
    {
    }
    ~Physics()
    {
    }

private:
    btSequentialImpulseConstraintSolver mConstraintSolver;
    btDefaultCollisionConfiguration mCollisionConfiguration;
    btCollisionDispatcher mDispatcher;
    btDbvtBroadphase mBroadphase;
public:
    btDiscreteDynamicsWorld mDynamicWorld;
};

