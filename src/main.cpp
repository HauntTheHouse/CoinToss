#include <iostream>
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <memory>

class WindowParameters
{
public:
    WindowParameters() = default;
    ~WindowParameters()
    {
        glfwDestroyWindow(mWindow);
    }

    void init(int aWidth, int aHeigth, const std::string& aName)
    {
        mWidth = aWidth;
        mHeight = aHeigth;
        mName = aName;
        mWindow = glfwCreateWindow(mWidth, mHeight, mName.c_str(), nullptr, nullptr);
    }

    GLFWwindow* mWindow{ nullptr };
    int mWidth{ 0 };
    int mHeight{ 0 };
    std::string mName;
};

template<typename T>
class Singleton
{
public:
    Singleton() = delete;
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

    static T& getInstance()
    {
        static T instance;
        return instance;
    }
};

void windowInit() noexcept
{
    if (!glfwInit())
    {
        std::cout << "Failed to init GLFW" << std::endl;
        return;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);

    Singleton<WindowParameters>::getInstance().init(800, 600, "MoneyHeist");

    if (!Singleton<WindowParameters>::getInstance().mWindow)
    {
        std::cout << "Failed to create GLFWwindow" << std::endl;
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(Singleton<WindowParameters>::getInstance().mWindow);
}

void gladInit() noexcept
{
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to init GLAD" << std::endl;
        return;
    }
}

int main()
{
    windowInit();
    gladInit();

    glViewport(0, 0, Singleton<WindowParameters>::getInstance().mWidth, Singleton<WindowParameters>::getInstance().mHeight);

    while (!glfwWindowShouldClose(Singleton<WindowParameters>::getInstance().mWindow))
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.6f, 0.5f, 1.0f, 1.0f);

        glViewport(0, 0, Singleton<WindowParameters>::getInstance().mWidth, Singleton<WindowParameters>::getInstance().mHeight);

        glfwPollEvents();
        glfwSwapBuffers(Singleton<WindowParameters>::getInstance().mWindow);
    }
    glfwTerminate();
}
