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
        System::getInstance().mWindowParameters.width = width;
        System::getInstance().mWindowParameters.height = height;

        Utils::calculateProjSpace();
    }

    void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
    {
        auto& camera = System::getInstance().mCamera;
        if (button == GLFW_MOUSE_BUTTON_LEFT)
        {
            if (action == GLFW_PRESS)
            {
                camera.leftMousePressed = true;
                glfwGetCursorPos(System::getInstance().mWindowParameters.window, &camera.oldPos.x, &camera.oldPos.y);
            }
            else if (action == GLFW_RELEASE)
            {
                camera.leftMousePressed = false;
            }
        }
        if (button == GLFW_MOUSE_BUTTON_RIGHT)
        {
            if (action == GLFW_PRESS)
            {
                camera.rightMousePressed = true;
                glfwGetCursorPos(System::getInstance().mWindowParameters.window, &camera.oldPos.x, &camera.oldPos.y);
            }
            else if (action == GLFW_RELEASE)
            {
                camera.rightMousePressed = false;
            }
        }
    }

    void scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
    {
        auto& camera = System::getInstance().mCamera;
        camera.radius -= yoffset * camera.zoomSensitivity;
        if (camera.radius < 0.15f)
            camera.radius = 0.15f;
        Utils::calculateViewSpace();
    }
}
