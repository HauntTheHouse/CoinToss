#include "DebugDraw.h"

#include "Shader.h"
#include "System.h"

void DebugDraw::init()
{
	const auto vertexShaderId = Shader::compileShader("shaders/debug_draw.vert", GL_VERTEX_SHADER);
	const auto fragmentShaderId = Shader::compileShader("shaders/debug_draw.frag", GL_FRAGMENT_SHADER);
	System::getData().mDebugDrawProgramId = Shader::createProgram({ vertexShaderId, fragmentShaderId });

	glGenVertexArrays(1, &mVAO);
	glGenBuffers(1, &mVBO);
}

void DebugDraw::render()
{
	glBindVertexArray(mVAO);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(DebugDrawVertex), &mVertices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(DebugDrawVertex), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(DebugDrawVertex), (void*)offsetof(DebugDrawVertex, color));

	Shader::setUniformMat4("uProjection", System::getProjective().mProjSpace);
	Shader::setUniformMat4("uView", System::getCamera().mViewSpace);

	glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(mVertices.size()));
	mVertices.clear();
}

void DebugDraw::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
{
	mVertices.push_back({ from, color });
	mVertices.push_back({ to, color });
}

int DebugDraw::getDebugMode() const
{
	return DBG_MAX_DEBUG_DRAW_MODE;
}
