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

private:
	std::vector<Vertex> mVertices;
	std::vector<GLuint> mIndices;
	std::vector<Texture> mTextures;

	GLuint mVAO, mVBO, mEBO;
};

enum aiTextureType;
struct aiScene;
struct aiNode;

class Model
{
public:
    Model() = default;
    Model(std::string aPath);

	void load(std::string aPath);
    void render(GLuint aProgramId) const;
	void clear();

	void setTransform(const glm::mat4& aTransform);

private:
	void processNode(const aiNode* aNode, const aiScene* aScene);

	struct TextureType
	{
		aiTextureType mType;
		std::string mUniformName;
	};
	std::vector<TextureType> mActiveTypes;

	std::vector<Mesh> mMeshes;
	std::string mDirectory;

	glm::mat4 mTransform;
};
