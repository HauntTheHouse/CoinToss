#pragma once

#include <string>
#include <vector>

#include <glad/glad.h>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

namespace Shader
{
	GLuint compileShader(const std::string& aPath, GLenum aShaderType) noexcept;
	GLuint createProgram(const std::vector<GLuint>& aShaderIds) noexcept;

	void setActiveProgramId(GLuint aProgramId) noexcept;
	GLuint getActiveProgramId() noexcept;

	void setUniformMat4(const GLchar* aName, const glm::mat4& aValue) noexcept;
	void setUniformVec3(const GLchar* aName, const glm::vec3& aValue) noexcept;
	void setUniformTexture(const GLchar* aName, GLint aTextureId) noexcept;
};

