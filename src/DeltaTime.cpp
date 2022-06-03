#include "DeltaTime.h"

#include <GLFW/glfw3.h>

double DeltaTime::mLastTime = 0.0;
double DeltaTime::mValue = 0.0;

void DeltaTime::calculate() noexcept
{
    double currentTime = glfwGetTime();
    mValue = currentTime - mLastTime;
    mLastTime = currentTime;
}

double DeltaTime::get() noexcept
{
    return mValue;
}
