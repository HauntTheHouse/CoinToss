#pragma once

#include <vector>
#include <glad/glad.h>
#include <LinearMath/btIDebugDraw.h>

class DebugDraw : public btIDebugDraw
{
public:
	DebugDraw() = default;

	void init();
	void render();

	void drawLine(const btVector3& from, const btVector3& to, const btVector3& color) override;
	int getDebugMode() const override;

	void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color) override {};
	void reportErrorWarning(const char* warningString) override {};
	void draw3dText(const btVector3& location, const char* textString) override {};
	void setDebugMode(int debugMode) override {};

private:
	GLuint mProgramId{ 0 };

	struct DebugDrawVertex
	{
		btVector3 position;
		btVector3 color;
	};
	std::vector<DebugDrawVertex> mVertices;

	GLuint mVAO{ 0 }, mVBO{ 0 };
};
