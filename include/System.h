#pragma once

#include "WindowParameters.h"
#include "Data.h"
#include "Camera.h"

class System
{
public:
    static WindowParameters& getWindowParameters() { return getInstance().mWindowParameters; }
    static Data& getData() { return getInstance().mData; }
    static Camera& getCamera() { return getInstance().mCamera; }
    static Projective& getProjective() { return getInstance().mProjective; }

private:
    System() = default;
    System(const System&) = delete;
    System& operator=(const System&) = delete;

    static System& getInstance()
    {
        static System instance;
        return instance;
    }

    WindowParameters mWindowParameters;
    Data mData;
    Camera mCamera;
    Projective mProjective;
};
