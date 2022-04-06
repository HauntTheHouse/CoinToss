#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "WindowParameters.h"
#include "Data.h"
#include "Singleton.h"
#include "Gui.h"

void glfwErrorCallback(int error, const char* description)
{
    std::cerr << "GLFW error " << error << ": " << description << std::endl;
}

inline void initGlfw() noexcept
{
    glfwSetErrorCallback(glfwErrorCallback);

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

inline void releaseMemory() noexcept
{
    Gui::shutdown();
    glfwTerminate();
}

int main()
{
    initGlfw();
    Singleton<WindowParameters>::getInstance().init(1000, 600, "MoneyHeist");
    Singleton<Data>::getInstance().init(glm::vec4(0.73f, 0.73f, 0.73f, 1.0f));
    initGlad();
    Gui::init();

    std::vector<GLfloat> platformVertices = {
        -0.5f,  0.5f, 0.0f,     0.0f, 1.0f, 0.0f,     0.0f,  0.0f,
        -0.5f, -0.5f, 0.0f,     0.0f, 1.0f, 0.0f,     0.0f,  1.0f,
         0.5f, -0.5f, 0.0f,     0.0f, 1.0f, 0.0f,     1.0f,  1.0f,
         0.5f,  0.5f, 0.0f,     0.0f, 1.0f, 0.0f,     1.0f,  0.0f,
    };
    std::vector<GLuint> platformIndices = {
        0, 2, 1,
        0, 3, 2,
    };

    GLuint VAO{ 0 }, VBO{ 0 }, EBO{ 0 };
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, platformVertices.size() * sizeof(GLfloat), platformVertices.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, platformIndices.size() * sizeof(GLuint), platformIndices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), reinterpret_cast<void*>(0 * sizeof(GLfloat)));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), reinterpret_cast<void*>(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), reinterpret_cast<void*>(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    std::string shaderStr;
    {
        std::ifstream shaderFile;
        shaderFile.open("shaders/object.vert");
        std::stringstream buffer;
        buffer << shaderFile.rdbuf();
        shaderStr = buffer.str();
    }
    auto vertexShaderStr = shaderStr.c_str();

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderStr, 0);
    glCompileShader(vertexShader);

    {
        std::ifstream shaderFile;
        shaderFile.open("shaders/object.frag");
        std::stringstream buffer;
        buffer << shaderFile.rdbuf();
        shaderStr = buffer.str();
    }
    auto fragmentShaderStr = shaderStr.c_str();

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderStr, 0);
    glCompileShader(fragmentShader);

    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    glDetachShader(program, vertexShader);
    glDetachShader(program, fragmentShader);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glViewport(0, 0, Singleton<WindowParameters>::getInstance().mWidth, Singleton<WindowParameters>::getInstance().mHeight);

    while (!glfwWindowShouldClose(Singleton<WindowParameters>::getInstance().mWindow))
    {
        Gui::newFrame();

        const auto& clearColor = Singleton<Data>::getInstance().mClearColor;
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);

        glViewport(0, 0, Singleton<WindowParameters>::getInstance().mWidth, Singleton<WindowParameters>::getInstance().mHeight);

        glUseProgram(program);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, platformIndices.size(), GL_UNSIGNED_INT, 0);

        Gui::fpsWindow();
        Gui::menuWindow();

        Gui::render();

        glfwPollEvents();
        glfwSwapBuffers(Singleton<WindowParameters>::getInstance().mWindow);
    }

    glDeleteBuffers(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    releaseMemory();

}
