#pragma once

#include <vector>
#include <glad/glad.h>
#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>

namespace Utils
{
	GLuint compileShader(const char* aPath, GLenum aShaderType) noexcept;
	GLuint createProgram(const std::vector<GLuint>& aShaderIds) noexcept;
	GLuint loadTexture(const char* aPath) noexcept;

	void calculateViewSpace() noexcept;
	void calculateProjSpace() noexcept;
	glm::vec2 calculateOffset() noexcept;

	void setUniformMat4(GLuint aProgramId, const GLchar* aName, const glm::mat4& aValue) noexcept;
}
