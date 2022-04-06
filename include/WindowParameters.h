#pragma once

#include <string>
#include <GLFW/glfw3.h>

struct WindowParameters
{
    WindowParameters() = default;
    ~WindowParameters();

    void init(int aWidth, int aHeigth, const std::string& aName);

    GLFWwindow* mWindow{ nullptr };
    int mWidth{ 0 };
    int mHeight{ 0 };
    std::string mName;
};
