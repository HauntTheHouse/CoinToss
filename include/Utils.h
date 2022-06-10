#pragma once

#include <vector>
#include <string>
#include <glad/glad.h>
#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>

#include "Model.h"

namespace Utils
{
	void initGlobals() noexcept;

	GLuint compileShader(const char* aPath, GLenum aShaderType) noexcept;
	GLuint createProgram(const std::vector<GLuint>& aShaderIds) noexcept;
	GLuint loadTexture(const char* aPath) noexcept;

	size_t getHash(const std::string& aString);

	enum class Axis
	{
		X, Y, Z
	};
	struct CylinderParameters
	{
		float radius;
		float height;
	};
	CylinderParameters calculateCylinderParameters(const ModelMeshes& aModelMeshes, Axis aAxis);

	void setUniformMat4(GLuint aProgramId, const GLchar* aName, const glm::mat4& aValue) noexcept;
}
