#pragma once

#include "WindowParameters.h"
#include "Data.h"
#include "Camera.h"

struct System
{
    static System& getInstance()
    {
        static System instance;
        return instance;
    }

    WindowParameters mWindowParameters;
    Data mData;
    Camera mCamera;

private:
    System() {};
    System(const System&) = delete;
    System& operator=(const System&) = delete;
};
