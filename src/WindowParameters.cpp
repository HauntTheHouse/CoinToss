#include "WindowParameters.h"

void WindowParameters::init(int aWidth, int aHeigth, const std::string& aName)
{
    width = aWidth;
    height = aHeigth;
    name = aName;
    window = glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);

    if (!window)
    {
        glfwTerminate();
        exit(-2);
    }

    glfwMakeContextCurrent(window);
}
