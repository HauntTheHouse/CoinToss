#include "WindowParameters.h"

#include <glad/glad.h>

void WindowParameters::init(int aWidth, int aHeigth, const std::string& aName)
{
    mWidth = aWidth;
    mHeight = aHeigth;
    mName = aName;

    if (!glfwInit()) exit(-1);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);

    if (!(mWindow = glfwCreateWindow(mWidth, mHeight, mName.c_str(), nullptr, nullptr)))
    {
        glfwTerminate();
        exit(-3);
    }

    glfwMakeContextCurrent(mWindow);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        glfwTerminate();
        exit(-2);
    }
}
