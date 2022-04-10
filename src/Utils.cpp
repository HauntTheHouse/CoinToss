#include "Utils.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image.h>

#include "System.h"

namespace Utils
{
    GLuint compileShader(const char* aPath, GLenum aShaderType) noexcept
    {
        std::ifstream shaderFile(aPath);
        std::stringstream buff;
        buff << shaderFile.rdbuf();
        const auto shaderStr = buff.str();
        const auto shaderCStr = shaderStr.c_str();

        const auto shaderId = glCreateShader(aShaderType);
        glShaderSource(shaderId, 1, &shaderCStr, 0);
        glCompileShader(shaderId);

        // check compiling errors
        GLint success;
        const GLsizei logSize = 256;
        GLchar infoLog[logSize];
        glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shaderId, logSize, nullptr, infoLog);
            std::cerr << "ERROR: Shader compilation failed\n" << infoLog << std::endl;
            exit(-4);
        }

        return shaderId;
    }

    GLuint createProgram(const std::vector<GLuint>& aShaderIds) noexcept
    {
        const auto program = glCreateProgram();

        for (auto shaderId : aShaderIds)
        {
            glAttachShader(program, shaderId);
        }

        glLinkProgram(program);

        for (auto shaderId : aShaderIds)
        {
            glDetachShader(program, shaderId);
            glDeleteShader(shaderId);
        }

        // check linking errors
        GLint success;
        const GLsizei logSize = 256;
        GLchar infoLog[logSize];
        glGetProgramiv(program, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(program, logSize, nullptr, infoLog);
            std::cout << "ERROR: Program linking failed\n" << infoLog << std::endl;
        }

        return program;
    }

    void calculateViewSpace() noexcept
    {
        auto& camera = System::getInstance().mCamera;
        const auto camX = sinf(glm::radians(camera.pitch)) * sinf(glm::radians(camera.yaw)) * camera.radius + camera.center.x;
        const auto camY = cosf(glm::radians(camera.pitch)) * camera.radius + camera.center.y;
        const auto camZ = sinf(glm::radians(camera.pitch)) * cosf(glm::radians(camera.yaw)) * camera.radius + camera.center.z;

        auto& data = System::getInstance().mData;
        data.viewSpace = glm::lookAt(glm::vec3(camX, camY, camZ), camera.center, camera.worldUp);
        setUniformMat4(data.programId, "uView", data.viewSpace);
    }

    void calculateProjSpace() noexcept
    {
        const auto aspect = System::getInstance().mWindowParameters.width / static_cast<float>(System::getInstance().mWindowParameters.height);
        System::getInstance().mData.projSpace = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 100.0f);
        setUniformMat4(System::getInstance().mData.programId, "uProjection", System::getInstance().mData.projSpace);
    }

    glm::vec2 calculateOffset() noexcept
    {
        double xPos, yPos;
        glfwGetCursorPos(System::getInstance().mWindowParameters.window, &xPos, &yPos);

        double xOffset = xPos - System::getInstance().mCamera.oldPos.x;
        double yOffset = yPos - System::getInstance().mCamera.oldPos.y;

        System::getInstance().mCamera.oldPos.x = xPos;
        System::getInstance().mCamera.oldPos.y = yPos;

        return glm::vec2(xOffset, yOffset);
    }

    GLuint loadTexture(const char* aPath) noexcept
    {
        int width, height, nrChannels;
        //stbi_set_flip_vertically_on_load(true);
        const auto textureData = stbi_load(aPath, &width, &height, &nrChannels, 0);
        if (!textureData)
        {
            std::cerr << "Texture upload was failed" << std::endl;
            exit(-5);
        }

        GLuint textureId;
        glGenTextures(1, &textureId);
        glBindTexture(GL_TEXTURE_2D, textureId);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        GLenum format;
        switch (nrChannels)
        {
        case 3:
            format = GL_RGB;
            break;
        case 4:
            format = GL_RGBA;
            break;
        default:
            format = GL_ZERO;
            break;
        }
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, textureData);
        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(textureData);

        return textureId;
    }

    void setUniformMat4(GLuint aProgramId, const GLchar* aName, const glm::mat4& aValue) noexcept
    {
        const auto location = glGetUniformLocation(aProgramId, aName);
        glUniformMatrix4fv(location, 1, GL_FALSE, reinterpret_cast<const GLfloat*>(&aValue));
    }
}
