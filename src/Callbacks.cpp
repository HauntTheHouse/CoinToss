#include "Callbacks.h"

#include <iostream>
#include "System.h"
#include "Utils.h"

namespace Glfw
{
    void errorCallback(int error, const char* description)
    {
        std::cerr << "GLFW error " << error << ": " << description << std::endl;
    }

    void widnowSizeCallback(GLFWwindow* window, int width, int height)
    {
        System::getWindowParameters().mWidth = width;
        System::getWindowParameters().mHeight = height;
        System::getProjective().calcProjSpace(System::getData().mProgramId);
    }

    void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
    {
        auto& camera = System::getCamera();
        if (button == GLFW_MOUSE_BUTTON_LEFT)
        {
            if (action == GLFW_PRESS)
            {
                System::getData().mLeftMousePressed = true;
                glfwGetCursorPos(System::getWindowParameters().mWindow, &camera.mOldPos.x, &camera.mOldPos.y);
            }
            else if (action == GLFW_RELEASE)
            {
                System::getData().mLeftMousePressed = false;
            }
        }
        if (button == GLFW_MOUSE_BUTTON_RIGHT)
        {
            if (action == GLFW_PRESS)
            {
                System::getData().mRightMousePressed = true;
                glfwGetCursorPos(System::getWindowParameters().mWindow, &camera.mOldPos.x, &camera.mOldPos.y);
            }
            else if (action == GLFW_RELEASE)
            {
                System::getData().mRightMousePressed = false;
            }
        }
    }

    void scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
    {
        auto& camera = System::getCamera();
        camera.zoomMove(yoffset);
        camera.calcViewSpace(System::getData().mProgramId);
    }
}
