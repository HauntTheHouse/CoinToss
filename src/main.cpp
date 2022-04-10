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

inline void initGlfw() noexcept;
inline void initGlad() noexcept;
inline void processInput() noexcept;
inline void render() noexcept;
inline void releaseMemory() noexcept;

int main()
{
    initGlfw();
    System::getInstance().mWindowParameters.init(1000, 600, "MoneyHeist");

    glfwSetWindowSizeCallback(System::getInstance().mWindowParameters.window, Glfw::widnowSizeCallback);
    glfwSetMouseButtonCallback(System::getInstance().mWindowParameters.window, Glfw::mouseButtonCallback);
    glfwSetScrollCallback(System::getInstance().mWindowParameters.window, Glfw::scrollCallback);

    initGlad();
    Gui::init();

    std::vector<GLfloat> platformVertices = {
        -1.0f,  0.0f,  1.0f,     0.0f, 1.0f, 0.0f,     0.0f, 0.0f,
        -1.0f,  0.0f, -1.0f,     0.0f, 1.0f, 0.0f,     0.0f, 1.0f,
         1.0f,  0.0f, -1.0f,     0.0f, 1.0f, 0.0f,     1.0f, 1.0f,
         1.0f,  0.0f,  1.0f,     0.0f, 1.0f, 0.0f,     1.0f, 0.0f,
    };
    std::vector<GLuint> platformIndices = {
        0, 2, 1,
        0, 3, 2,
    };

    auto& data = System::getInstance().mData;

    glGenVertexArrays(1, &data.platformVAO);
    glBindVertexArray(data.platformVAO);

    glGenBuffers(1, &data.platformVBO);
    glBindBuffer(GL_ARRAY_BUFFER, data.platformVBO);
    glBufferData(GL_ARRAY_BUFFER, platformVertices.size() * sizeof(GLfloat), platformVertices.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &data.platformEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, data.platformEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, platformIndices.size() * sizeof(GLuint), platformIndices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), reinterpret_cast<void*>(0 * sizeof(GLfloat)));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), reinterpret_cast<void*>(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), reinterpret_cast<void*>(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    const auto vertexShaderId = Utils::compileShader("shaders/object.vert", GL_VERTEX_SHADER);
    const auto fragmentShaderId = Utils::compileShader("shaders/object.frag", GL_FRAGMENT_SHADER);
    data.programId = Utils::createProgram({ vertexShaderId, fragmentShaderId });

    data.clearColor = glm::vec4(0.8f, 0.8f, 0.9f, 1.0f);

    glUseProgram(data.programId);

    Utils::calculateProjSpace();
    Utils::calculateViewSpace();

    data.platformModelSpace = glm::mat4(1.0f);
    Utils::setUniformMat4(data.programId, "uModel", data.platformModelSpace);

    const auto textureId = Utils::loadTexture("images/wood_texture3854.jpg");
    glActiveTexture(GL_TEXTURE0);
    glUniform1i(glGetUniformLocation(data.programId, "uWoodTexture"), 0);

    glViewport(0, 0, System::getInstance().mWindowParameters.width, System::getInstance().mWindowParameters.height);

    while (!glfwWindowShouldClose(System::getInstance().mWindowParameters.window))
    {
        Gui::newFrame();
        processInput();

        glClear(GL_COLOR_BUFFER_BIT);
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



inline void initGlfw() noexcept
{
    glfwSetErrorCallback(Glfw::errorCallback);

    if (!glfwInit()) exit(-1);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
}

inline void initGlad() noexcept
{
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to init GLAD" << std::endl;
        exit(-3);
    }
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
    Gui::render();

    glUseProgram(System::getInstance().mData.programId);
    glBindVertexArray(System::getInstance().mData.platformVAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

inline void releaseMemory() noexcept
{
    Gui::shutdown();

    glDeleteBuffers(1, &System::getInstance().mData.platformEBO);
    glDeleteBuffers(1, &System::getInstance().mData.platformVBO);
    glDeleteBuffers(1, &System::getInstance().mData.platformVAO);

    glfwDestroyWindow(System::getInstance().mWindowParameters.window);
    glfwTerminate();
}
