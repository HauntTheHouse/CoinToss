#include "WindowParameters.h"

void WindowParameters::init(int aWidth, int aHeigth, const std::string& aName)
{
    mWidth = aWidth;
    mHeight = aHeigth;
    mName = aName;
    mWindow = glfwCreateWindow(mWidth, mHeight, mName.c_str(), nullptr, nullptr);

    if (!mWindow)
    {
        glfwTerminate();
        exit(-2);
    }

    glfwMakeContextCurrent(mWindow);
}
