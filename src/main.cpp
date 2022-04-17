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

    auto& data = System::getInstance().mData;

    data.model.load("models/table/RoundTable.obj");
    //model2.load("models/coin/PirateCoin.obj");
    //model.load("models/backpack/backpack.obj");

    const auto vertexShaderId = Utils::compileShader("shaders/object.vert", GL_VERTEX_SHADER);
    const auto fragmentShaderId = Utils::compileShader("shaders/object.frag", GL_FRAGMENT_SHADER);
    data.programId = Utils::createProgram({ vertexShaderId, fragmentShaderId });

    data.clearColor = glm::vec4(0.8f, 0.8f, 0.9f, 1.0f);

    glUseProgram(data.programId);

    Utils::calculateProjSpace();
    Utils::calculateViewSpace();

    data.platformModelSpace = glm::mat4(1.0f);
    Utils::setUniformMat4(data.programId, "uModel", data.platformModelSpace);

    glViewport(0, 0, System::getInstance().mWindowParameters.width, System::getInstance().mWindowParameters.height);

    while (!glfwWindowShouldClose(System::getInstance().mWindowParameters.window))
    {
        Gui::newFrame();
        processInput();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(data.clearColor.x, data.clearColor.y, data.clearColor.z, data.clearColor.w);

        glViewport(0, 0, System::getInstance().mWindowParameters.width, System::getInstance().mWindowParameters.height);

        Gui::fpsWindow();
        Gui::menuWindow();

        render();

        glfwPollEvents();
        glfwSwapBuffers(System::getInstance().mWindowParameters.window);
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

    System::getInstance().mWindowParameters.init(1000, 600, "MoneyHeist");

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to init GLAD" << std::endl;
        exit(-3);
    }

    glfwSetWindowSizeCallback(System::getInstance().mWindowParameters.window, Glfw::widnowSizeCallback);
    glfwSetMouseButtonCallback(System::getInstance().mWindowParameters.window, Glfw::mouseButtonCallback);
    glfwSetScrollCallback(System::getInstance().mWindowParameters.window, Glfw::scrollCallback);

    glEnable(GL_DEPTH_TEST);

    Gui::init();
}

inline void processInput() noexcept
{
    auto& camera = System::getInstance().mCamera;
    if (camera.leftMousePressed)
    {
        const auto offset = Utils::calculateOffset();

        camera.yaw -= offset.x;
        camera.pitch -= offset.y;

        if (camera.pitch <= 0.0f)
            camera.pitch = 1.0f;
        else if (camera.pitch >= 180.0f)
            camera.pitch = 179.0f;

        Utils::calculateViewSpace();
    }
    if (camera.rightMousePressed)
    {
        const auto offset = Utils::calculateOffset();

        auto centerViewSpace = System::getInstance().mData.viewSpace * glm::vec4(camera.center, 1.0f);
        centerViewSpace.x -= offset.x * camera.moveSensitivity;
        centerViewSpace.y += offset.y * camera.moveSensitivity;
        camera.center = glm::inverse(System::getInstance().mData.viewSpace) * centerViewSpace;

        Utils::calculateViewSpace();
    }
}

inline void render() noexcept
{
    System::getInstance().mData.model.draw(System::getInstance().mData.programId);
    //model2.draw(System::getInstance().mData.programId);

    Gui::render();
}

inline void releaseMemory() noexcept
{
    Gui::shutdown();

    System::getInstance().mData.model.clear();

    glfwDestroyWindow(System::getInstance().mWindowParameters.window);
    glfwTerminate();
}
