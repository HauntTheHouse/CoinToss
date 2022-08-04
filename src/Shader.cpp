#include "Shader.h"

#include <iostream>
#include <fstream>
#include <sstream>

#include "System.h"

namespace Shader
{
    namespace
    {
        GLuint mActiveProgramId;
    }

    GLuint compileShader(const std::string& aPath, GLenum aShaderType) noexcept
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

    void setActiveProgramId(GLuint aProgramId) noexcept
    {
        mActiveProgramId = aProgramId;
        glUseProgram(mActiveProgramId);
    }

    GLuint getActiveProgramId() noexcept
    {
        return mActiveProgramId;
    }

    void setUniformFloat(const GLchar* aName, float aValue) noexcept
    {
        const auto location = glGetUniformLocation(Shader::getActiveProgramId(), aName);
        glUniform1f(location, static_cast<GLfloat>(aValue));
    }

    void setUniformMat4(const GLchar* aName, const glm::mat4& aValue) noexcept
    {
        const auto location = glGetUniformLocation(Shader::getActiveProgramId(), aName);
        glUniformMatrix4fv(location, 1, GL_FALSE, reinterpret_cast<const GLfloat*>(&aValue));
    }

    void setUniformVec3(const GLchar* aName, const glm::vec3& aValue) noexcept
    {
        const auto location = glGetUniformLocation(Shader::getActiveProgramId(), aName);
        glUniform3f(location, static_cast<GLfloat>(aValue.x), static_cast<GLfloat>(aValue.y), static_cast<GLfloat>(aValue.z));
    }

    void setUniformTexture(const GLchar* aName, GLint aTextureId) noexcept
    {
        const auto location = glGetUniformLocation(Shader::getActiveProgramId(), aName);
        glUniform1i(location, aTextureId);
    }
}
