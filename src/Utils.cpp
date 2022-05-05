#include "Utils.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <json/json.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image.h>

#include "System.h"

namespace Utils
{
    void initGlobals() noexcept
    {
        Json::Value pref;
        {
            std::ifstream prefStream("preferences.json");
            prefStream >> pref;
        }

        const auto name   = pref["window-parameters"].get("name", "").asString();
        const auto width  = pref["window-parameters"].get("width", 800).asInt();
        const auto height = pref["window-parameters"].get("height", 600).asInt();
        System::getWindowParameters().init(width, height, name);

        auto& camera = System::getCamera();
        camera.mRadius          = pref["camera"].get("radius", 3.0f).asFloat();
        camera.mPitch           = pref["camera"].get("pitch", 45.0f).asFloat();
        camera.mYaw             = pref["camera"].get("yaw", 0.0f).asFloat();
        camera.mRoundMoveSensitivity = pref["camera"].get("round-move-sensitivity", 0.5f).asFloat();
        camera.mFlatMoveSensitivity = pref["camera"].get("flat-move-sensitivity", 0.005f).asFloat();
        camera.mZoomSensitivity = pref["camera"].get("zoom-sensitivity", 0.1f).asFloat();

        const auto& center = pref["camera"]["center"];
        for (int i = 0; i < center.size(); ++i)
        {
            System::getCamera().mCenter[i] = center[i].asFloat();
        }
        const auto& worldUp = pref["camera"]["world-up"];
        for (int i = 0; i < worldUp.size(); ++i)
        {
            System::getCamera().mWorldUp[i] = worldUp[i].asFloat();
        }

        auto& proj = System::getProjective();
        proj.mFovy = pref["projective"].get("fovy", 45).asFloat();
        proj.mNear = pref["projective"].get("near", 0.1).asFloat();
        proj.mFar  = pref["projective"].get("far", 100).asFloat();

        const auto& color = pref["clear-color"];
        for (int i = 0; i < color.size(); ++i)
        {
            System::getData().mClearColor[i] = color[i].asFloat();
        }

        const auto& models = pref["models"];
        System::getData().mModels.resize(models.size());
        for (int i = 0; i < models.size(); ++i)
        {
            System::getData().mModels[i].load(models[i].get("path", "").asString());

            glm::vec3 position;
            const auto& posJson = models[i]["position"];
            for (int j = 0; j < posJson.size(); ++j)
            {
                position[j] = posJson[j].asFloat();
            }

            glm::mat4 transform(1.0f);
            transform = glm::translate(transform, position);
            System::getData().mModels[i].setTransform(transform);
        }
    }

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

    glm::vec2 calculateOffset() noexcept
    {
        double xPos, yPos;
        glfwGetCursorPos(System::getWindowParameters().mWindow, &xPos, &yPos);

        double xOffset = xPos - System::getCamera().mOldPos.x;
        double yOffset = yPos - System::getCamera().mOldPos.y;

        System::getCamera().mOldPos.x = xPos;
        System::getCamera().mOldPos.y = yPos;

        return glm::vec2(xOffset, yOffset);
    }

    GLuint loadTexture(const char* aPath) noexcept
    {
        int width, height, numChannels;
        //stbi_set_flip_vertically_on_load(true);
        const auto textureData = stbi_load(aPath, &width, &height, &numChannels, 0);
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
        if (numChannels == 1)
            format = GL_RED;
        else if (numChannels == 2)
            format = GL_RG;
        else if (numChannels == 3)
            format = GL_RGB;
        else if (numChannels == 4)
            format = GL_RGBA;

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
