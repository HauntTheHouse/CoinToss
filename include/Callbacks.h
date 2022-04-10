#pragma once

#include <GLFW/glfw3.h>

namespace Glfw
{
	void errorCallback(int error, const char* description);
	void widnowSizeCallback(GLFWwindow* window, int width, int height);
	void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
}
