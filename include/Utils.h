#pragma once

#include <vector>
#include <string>
#include <glad/glad.h>
#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>

namespace Utils
{
	void initGlobals() noexcept;

	GLuint compileShader(const char* aPath, GLenum aShaderType) noexcept;
	GLuint createProgram(const std::vector<GLuint>& aShaderIds) noexcept;
	GLuint loadTexture(const char* aPath) noexcept;

	size_t getHash(const std::string& aString);

	void setUniformMat4(GLuint aProgramId, const GLchar* aName, const glm::mat4& aValue) noexcept;
}
