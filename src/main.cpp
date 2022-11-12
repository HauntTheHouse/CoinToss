#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <btBulletDynamicsCommon.h>
#include <glm/glm.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "Callbacks.h"
#include "System.h"
#include "Gui.h"
#include "Utils.h"
#include "Shader.h"
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

    auto vertexShaderId = Shader::compileShader("shaders/object.vert", GL_VERTEX_SHADER);
    auto fragmentShaderId = Shader::compileShader("shaders/object.frag", GL_FRAGMENT_SHADER);
    data.mModelsProgramId = Shader::createProgram({vertexShaderId, fragmentShaderId});

    vertexShaderId = Shader::compileShader("shaders/light_depth.vert", GL_VERTEX_SHADER);
    fragmentShaderId = Shader::compileShader("shaders/light_depth.frag", GL_FRAGMENT_SHADER);
    data.mDepthProgramId = Shader::createProgram({ vertexShaderId, fragmentShaderId });

    vertexShaderId = Shader::compileShader("shaders/depth_map_to_color_map.vert", GL_VERTEX_SHADER);
    fragmentShaderId = Shader::compileShader("shaders/depth_map_to_color_map.frag", GL_FRAGMENT_SHADER);
    data.mDepthToColorProgramId = Shader::createProgram({ vertexShaderId, fragmentShaderId });


    Shader::setActiveProgramId(data.mModelsProgramId);

    System::getProjective().calcProjSpace();
    Shader::setUniformMat4("uProjection", System::getProjective().mProjSpace);

    System::getCamera().calcViewSpace();
    Shader::setUniformMat4("uView", System::getCamera().mViewSpace);
    Shader::setUniformVec3("uCameraPosition", System::getCamera().mCamPosition);

    Shader::setUniformVec3("uLightColor", System::getData().mLightColor);
    Shader::setUniformVec3("uLightDir", System::getData().mLightDir);


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

        static glm::mat4 lightProjection = glm::ortho(-1.5f, 1.5f, -1.5f, 1.5f, 1.0f, 5.0f);
        glm::mat4 lightView = glm::lookAt(-data.mLightDir * 2.0f, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 lightSpaceMatrix = lightProjection * lightView;
        Shader::setActiveProgramId(data.mDepthProgramId);
        Shader::setUniformMat4("uLightSpace", lightSpaceMatrix);

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
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    System::getData().mPlane.init();

    System::getData().mDepthFramebuffer.init(glm::vec2(1024), Framebuffer::Mode::DEPTH);
    System::getData().mDepthToColorFramebuffer.init(glm::vec2(256), Framebuffer::Mode::RGB);

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
        auto offset = camera.calcOffset();
        camera.roundMove(std::move(offset));
        camera.updateCamera();
    }
    else if (System::getData().mRightMousePressed)
    {
        auto offset = camera.calcOffset();
        camera.flatMove(std::move(offset));
        camera.updateCamera();
    }
}

inline void render() noexcept
{
    auto& data = System::getData();

    // Render scene to depth texture;
    Shader::setActiveProgramId(data.mDepthProgramId);
    data.mDepthFramebuffer.bind();
        for (const auto& model : System::getData().mModels)
        {
            model.render();
        }
    data.mDepthFramebuffer.unbind();

    // Convert depth texture to RGB texture
    Shader::setActiveProgramId(data.mDepthToColorProgramId);
    data.mDepthToColorFramebuffer.bind();
        Shader::setUniformTexture("uDepthMap", 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, data.mDepthFramebuffer.getTexture());
        data.mPlane.render();
    data.mDepthToColorFramebuffer.unbind();

    // Render scene
    Shader::setActiveProgramId(data.mModelsProgramId);
    glViewport(0, 0, System::getWindowParameters().mWidth, System::getWindowParameters().mHeight);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (const auto& model : data.mModels)
    {
        model.render();
    }

    // Render physics shapes
    System::getPhysics().mDynamicWorld.debugDrawWorld();
    data.mDebugDraw.render();

    // Render GUI
    Gui::render();
}

inline void releaseMemory() noexcept
{
    Gui::shutdown();

    System::getData().mDepthToColorFramebuffer.clear();
    System::getData().mDepthFramebuffer.clear();

    System::getData().mPlane.clear();
    for (auto& model : System::getData().mModels)
    {
        model.clear();
    }

    glDeleteProgram(System::getData().mModelsProgramId);
    glDeleteProgram(System::getData().mDepthProgramId);
    glDeleteProgram(System::getData().mDepthToColorProgramId);

    glfwDestroyWindow(System::getWindowParameters().mWindow);
    glfwTerminate();
}
