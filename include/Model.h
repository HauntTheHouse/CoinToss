#pragma once

#include <vector>
#include <string>

#include <glad/glad.h>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

struct Vertex
{
	glm::vec3 mPosition;
	glm::vec3 mNormal;
	glm::vec2 mTexCoords;
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
	void render(GLuint aProgramId) const;
	void clear();
	const std::vector<Vertex>& getVertices() const { return mVertices; }

private:
	std::vector<Vertex> mVertices;
	std::vector<GLuint> mIndices;
	std::vector<Texture> mTextures;

	GLuint mVAO, mVBO, mEBO;
};

using ModelMeshes = std::vector<Mesh>;

enum aiTextureType;
struct aiScene;
struct aiNode;

class ModelLoader
{
public:
	ModelMeshes load(const std::string& aPath);

private:
	void processNode(const aiNode* aNode, ModelMeshes& aModelMeshes);

	struct TextureType
	{
		aiTextureType mType;
		std::string mUniformName;
	};
	std::vector<TextureType> mActiveTypes;

	const aiScene* mScene;

	std::string mDirectory;
};

class Model
{
public:
    Model() = default;
    Model(size_t aModelMeshesId);

    void render(GLuint aProgramId) const;
	void clear();

	size_t mId;
	glm::mat4 mTransform;
};
