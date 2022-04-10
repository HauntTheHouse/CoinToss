#pragma once

#include <string>
#include <GLFW/glfw3.h>

struct WindowParameters
{
    WindowParameters() = default;
    ~WindowParameters();

    void init(int aWidth, int aHeigth, const std::string& aName);

    GLFWwindow* window{ nullptr };
    int width{ 0 };
    int height{ 0 };
    std::string name;
};
