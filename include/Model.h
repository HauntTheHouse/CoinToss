#pragma once

#include <vector>
#include <string>
#include <memory>

#include <glad/glad.h>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <assimp/material.h>
#include <assimp/scene.h>
#include <BulletCollision/CollisionShapes/btCollisionShape.h>
#include <BulletDynamics/Dynamics/btRigidBody.h>

#include <MotionState.h>

struct Vertex
{
	glm::vec3 mPosition;
	glm::vec3 mNormal;
	glm::vec2 mTexCoords;
	glm::vec3 mTangent;
	glm::vec3 mBitangent;
};

struct Texture
{
	unsigned int mId;
	std::string mUniformName;
};

struct Mesh
{
	Mesh(std::vector<Vertex> aVertices, std::vector<unsigned int> aIndices, std::vector<Texture> aTextures);
	~Mesh() = default;
	void render() const;
	void clear();
	const std::vector<Vertex>& getVertices() const { return mVertices; }

private:
	std::vector<Vertex> mVertices;
	std::vector<GLuint> mIndices;
	std::vector<Texture> mTextures;

	GLuint mVAO, mVBO, mEBO;
};

using ModelMeshes = std::vector<Mesh>;

struct ModelsData
{
	glm::vec3 mAmbientColor;
	glm::vec3 mDiffuseColor;
	glm::vec3 mSpecularColor;
	float mSpecularExponent;

	ModelMeshes mModelMeshes;
};

class ModelLoader
{
public:
	static ModelsData load(const std::string& aPath);

private:
	ModelLoader(const std::string& aPath);

	void processNode(const aiNode* aNode);

	struct TextureType
	{
		aiTextureType mType;
		std::string mUniformName;
	};
	std::vector<TextureType> mActiveTypes;

	const aiScene* mScene;
	ModelsData mModelsData;
	std::string mDirectory;
};

class Model
{
public:
    Model() = default;

    void render() const;
	void clear();

	size_t mId;

	std::shared_ptr<ModelsData> mModelsData;
	std::shared_ptr<btCollisionShape> mCollisionShape;
	std::unique_ptr<btRigidBody> mRigidBody;
	std::unique_ptr<MotionState> mMotionState;
};
