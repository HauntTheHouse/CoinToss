#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <btBulletDynamicsCommon.h>

#include "Callbacks.h"
#include "System.h"
#include "Gui.h"
#include "Utils.h"
#include "Model.h"
#include "MotionState.h"
#include "DeltaTime.h"
#include "DebugDraw.h"

inline void init() noexcept;
inline void processInput() noexcept;
inline void render() noexcept;
inline void releaseMemory() noexcept;

int main()
{
    init();

    auto& data = System::getData();

    const auto vertexShaderId = Utils::compileShader("shaders/object.vert", GL_VERTEX_SHADER);
    const auto fragmentShaderId = Utils::compileShader("shaders/object.frag", GL_FRAGMENT_SHADER);
    data.mProgramId = Utils::createProgram({ vertexShaderId, fragmentShaderId });

    glUseProgram(data.mProgramId);

    System::getProjective().calcProjSpace(data.mProgramId);
    System::getCamera().calcViewSpace(data.mProgramId);

    glViewport(0, 0, System::getWindowParameters().mWidth, System::getWindowParameters().mHeight);

    while (!glfwWindowShouldClose(System::getWindowParameters().mWindow))
    {
        DeltaTime::calculate();
        Gui::newFrame();
        processInput();
        System::getPhysics().mDynamicWorld.stepSimulation(static_cast<btScalar>(DeltaTime::get()), 10, 1.0f/600.0f);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(data.mClearColor.r, data.mClearColor.g, data.mClearColor.b, data.mClearColor.a);

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

    Utils::initGlobals();

    glfwSetWindowSizeCallback(System::getWindowParameters().mWindow, Glfw::widnowSizeCallback);
    glfwSetMouseButtonCallback(System::getWindowParameters().mWindow, Glfw::mouseButtonCallback);
    glfwSetScrollCallback(System::getWindowParameters().mWindow, Glfw::scrollCallback);

    glEnable(GL_DEPTH_TEST);

    Gui::init();

    System::getPhysics().mDynamicWorld.setGravity(btVector3(BT_ZERO, btScalar(-9.8f), BT_ZERO));
    System::getData().mDebugDraw.init();
    System::getPhysics().mDynamicWorld.setDebugDrawer(&System::getData().mDebugDraw);
}

inline void processInput() noexcept
{
    if (Gui::isMenuActive())
        return;

    auto& camera = System::getCamera();
    if (System::getData().mLeftMousePressed)
    {
        const auto offset = camera.calcOffset();
        camera.roundMove(offset);
        camera.calcViewSpace(System::getData().mProgramId);
    }
    else if (System::getData().mRightMousePressed)
    {
        const auto offset = camera.calcOffset();
        camera.flatMove(offset);
        camera.calcViewSpace(System::getData().mProgramId);
    }
}

inline void render() noexcept
{
    glUseProgram(System::getData().mProgramId);
    for (const auto& model : System::getData().mModels)
    {
        model.render(System::getData().mProgramId);
    }

    System::getPhysics().mDynamicWorld.debugDrawWorld();
    System::getData().mDebugDraw.render();

    Gui::render();
}

inline void releaseMemory() noexcept
{
    Gui::shutdown();

    for (auto& model : System::getData().mModels)
    {
        model.clear();
    }

    glfwDestroyWindow(System::getWindowParameters().mWindow);
    glfwTerminate();
}
