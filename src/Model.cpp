#include "Model.h"

#include <assimp/Importer.hpp>
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <glm/gtc/matrix_transform.hpp>

#include "Utils.h"
#include "System.h"

Model::Model(size_t aModelMeshesId)
    : mModelMeshesId(aModelMeshesId)
{
}

void Model::render(GLuint aProgramId) const
{
    Utils::setUniformMat4(aProgramId, "uModel", mTransform);

    for (const auto& mesh : System::getData().mModelMeshesContainer[mModelMeshesId])
    {
        mesh.render(aProgramId);
    }
}

void Model::clear()
{
    for (auto& mesh : System::getData().mModelMeshesContainer[mModelMeshesId])
    {
        mesh.clear();
    }
}

void Model::setTransform(const glm::mat4& aTransform)
{
    mTransform = aTransform;
}

ModelMeshes ModelLoader::load(const std::string& aPath)
{
    mDirectory = (aPath.substr(0, aPath.find_last_of('/') + 1));

    mActiveTypes = {
        { aiTextureType_DIFFUSE,  "uTextureDiffuse" },
        { aiTextureType_SPECULAR, "uTextureSpecular" },
        //{ aiTextureType_HEIGHT, "textureNormal" }
    };

    Assimp::Importer importer;
    mScene = importer.ReadFile(aPath.c_str(), aiProcessPreset_TargetRealtime_MaxQuality);

    ModelMeshes modelMeshes;
    processNode(mScene->mRootNode, modelMeshes);

    return modelMeshes;
}

void ModelLoader::processNode(const aiNode* aNode, ModelMeshes& aModelMeshes)
{
    for (size_t i = 0; i < aNode->mNumMeshes; ++i)
    {
        // Add vertices
        std::vector<Vertex> vertices;

        const auto meshId = aNode->mMeshes[i];
        const auto mesh = mScene->mMeshes[meshId];

        for (size_t i = 0; i < mesh->mNumVertices; ++i)
        {
            Vertex tmpVertex;

            const auto& vertex = mesh->mVertices[i];
            const auto& normal = mesh->mNormals[i];
            const auto& texCoords = mesh->mTextureCoords[0][i];

            tmpVertex.mPosition = glm::vec3(vertex.x, vertex.y, vertex.z);
            tmpVertex.mNormal = glm::vec3(normal.x, normal.y, normal.z);
            tmpVertex.mTexCoords = glm::vec2(texCoords.x, texCoords.y);

            vertices.emplace_back(std::move(tmpVertex));
        }

        // Add indices
        std::vector<GLuint> indices;

        for (size_t i = 0; i < mesh->mNumFaces; ++i)
        {
            const auto& face = mesh->mFaces[i];
            for (size_t j = 0; j < face.mNumIndices; ++j)
            {
                indices.emplace_back(face.mIndices[j]);
            }
        }

        // Add textures
        std::vector<Texture> textures;

        const auto materialId = mesh->mMaterialIndex;
        const auto& material = mScene->mMaterials[materialId];

        // We iterates through all types of texture and if we have already downloaded
        // it we remove the texture from mActiveTypes. This is necessary so that we
        // don't load the same texture several times for each mesh.
        for (auto it = mActiveTypes.begin(); it != mActiveTypes.end(); )
        {
            if (material->GetTextureCount(it->mType))
            {
                Texture tmpTexture;
                for (size_t i = 0; i < material->GetTextureCount(it->mType); ++i)
                {
                    aiString str;
                    const auto texture = material->GetTexture(it->mType, i, &str);
                    const auto texturePath = mDirectory + str.C_Str();

                    tmpTexture.mId = Utils::loadTexture(texturePath.c_str());
                    tmpTexture.mUniformName = it->mUniformName;
                }
                it = mActiveTypes.erase(it);
                textures.emplace_back(std::move(tmpTexture));
            }
            else
            {
                it++;
            }
        }

        aModelMeshes.emplace_back(Mesh(vertices, indices, textures));
    }

    for (size_t i = 0; i < aNode->mNumChildren; ++i)
    {
        processNode(aNode->mChildren[i], aModelMeshes);
    }
}

Mesh::Mesh(std::vector<Vertex> aVertices, std::vector<unsigned int> aIndices, std::vector<Texture> aTextures)
    : mVertices(std::move(aVertices))
    , mIndices(std::move(aIndices))
    , mTextures(std::move(aTextures))
{
    glGenVertexArrays(1, &mVAO);
    glGenBuffers(1, &mVBO);
    glGenBuffers(1, &mEBO);

    glBindVertexArray(mVAO);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);

    glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(Vertex), &mVertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size() * sizeof(unsigned int), &mIndices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, mNormal));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, mTexCoords));
}

void Mesh::render(GLuint aProgramId) const
{
    for (size_t i = 0; i < mTextures.size(); ++i)
    {
        glActiveTexture(GL_TEXTURE0 + i);

        glUniform1i(glGetUniformLocation(aProgramId, mTextures[i].mUniformName.c_str()), i);
        glBindTexture(GL_TEXTURE_2D, mTextures[i].mId);
    }

    glBindVertexArray(mVAO);
    glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, nullptr);
}

void Mesh::clear()
{
    mVertices.clear();
    mIndices.clear();
    mTextures.clear();

    glDeleteBuffers(1, &mEBO);
    glDeleteBuffers(1, &mVBO);
    glDeleteBuffers(1, &mVAO);
}
