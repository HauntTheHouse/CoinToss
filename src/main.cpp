#include <iostream>
#include <vector>
#include <ctime>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Callbacks.h"
#include "System.h"
#include "Gui.h"
#include "Utils.h"
#include "Model.h"

inline void init() noexcept;
inline void processInput() noexcept;
inline void render() noexcept;
inline void releaseMemory() noexcept;

int main()
{
    init();

    auto& data = System::getData();

    data.mModel.load("models/table/RoundTable.obj");
    //model2.load("models/coin/PirateCoin.obj");
    //model.load("models/backpack/backpack.obj");

    const auto vertexShaderId = Utils::compileShader("shaders/object.vert", GL_VERTEX_SHADER);
    const auto fragmentShaderId = Utils::compileShader("shaders/object.frag", GL_FRAGMENT_SHADER);
    data.mProgramId = Utils::createProgram({ vertexShaderId, fragmentShaderId });

    glUseProgram(data.mProgramId);

    System::getProjective().calcProjSpace(data.mProgramId);
    System::getCamera().calcViewSpace(data.mProgramId);

    data.mPlatformModelSpace = glm::mat4(1.0f);
    Utils::setUniformMat4(data.mProgramId, "uModel", data.mPlatformModelSpace);

    glViewport(0, 0, System::getWindowParameters().mWidth, System::getWindowParameters().mHeight);

    while (!glfwWindowShouldClose(System::getWindowParameters().mWindow))
    {
        Gui::newFrame();
        processInput();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(data.mClearColor.x, data.mClearColor.y, data.mClearColor.z, data.mClearColor.w);

        glViewport(0, 0, System::getWindowParameters().mWidth, System::getWindowParameters().mHeight);

        Gui::fpsWindow();
        Gui::menuWindow();

        render();

        glfwPollEvents();
        glfwSwapBuffers(System::getWindowParameters().mWindow);
    }

    releaseMemory();
    return 0;
}

inline void init() noexcept
{
    glfwSetErrorCallback(Glfw::errorCallback);

    if (!glfwInit()) exit(-1);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);

    Utils::initGlobals();

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to init GLAD" << std::endl;
        exit(-3);
    }

    glfwSetWindowSizeCallback(System::getWindowParameters().mWindow, Glfw::widnowSizeCallback);
    glfwSetMouseButtonCallback(System::getWindowParameters().mWindow, Glfw::mouseButtonCallback);
    glfwSetScrollCallback(System::getWindowParameters().mWindow, Glfw::scrollCallback);

    glEnable(GL_DEPTH_TEST);

    Gui::init();
}

inline void processInput() noexcept
{
    auto& camera = System::getCamera();
    if (System::getData().mLeftMousePressed)
    {
        const auto offset = Utils::calculateOffset();
        camera.roundMove(offset);
        camera.calcViewSpace(System::getData().mProgramId);
    }
    if (System::getData().mRightMousePressed)
    {
        const auto offset = Utils::calculateOffset();
        camera.flatMove(offset);
        camera.calcViewSpace(System::getData().mProgramId);
    }
}

inline void render() noexcept
{
    System::getData().mModel.draw(System::getData().mProgramId);
    //model2.draw(System::getData().mProgramId);

    Gui::render();
}

inline void releaseMemory() noexcept
{
    Gui::shutdown();

    System::getData().mModel.clear();

    glfwDestroyWindow(System::getWindowParameters().mWindow);
    glfwTerminate();
}
