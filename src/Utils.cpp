#include "Utils.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <limits>

#include <json/json.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image.h>

#include <btBulletCollisionCommon.h>

#include "System.h"

namespace Utils
{
    namespace
    {
        template <typename Vec>
        Vec readVector(const Json::Value& aJsonVector)
        {
            assert(Vec::length() == aJsonVector.size());
            Vec vec{};
            for (Json::Value::ArrayIndex i = 0; i < aJsonVector.size(); ++i)
            {
                vec[i] = aJsonVector[i].asFloat();
            }
            return vec;
        }
    }

    void initGlobals() noexcept
    {
        Json::Value pref;
        {
            std::ifstream prefStream("preferences.json");
            prefStream >> pref;
        }

        const auto name   = pref["window-parameters"].get("name", "").asString();
        const auto width  = pref["window-parameters"].get("width", 800).asInt();
        const auto height = pref["window-parameters"].get("height", 600).asInt();
        System::getWindowParameters().init(width, height, name);

        auto& camera = System::getCamera();
        camera.mRadius          = pref["camera"].get("radius", 3.0f).asFloat();
        camera.mPitch           = pref["camera"].get("pitch", 45.0f).asFloat();
        camera.mYaw             = pref["camera"].get("yaw", 0.0f).asFloat();
        camera.mRoundMoveSensitivity = pref["camera"].get("round-move-sensitivity", 0.5f).asFloat();
        camera.mFlatMoveSensitivity = pref["camera"].get("flat-move-sensitivity", 0.005f).asFloat();
        camera.mZoomSensitivity = pref["camera"].get("zoom-sensitivity", 0.1f).asFloat();

        System::getCamera().mCenter  = readVector<glm::vec3>(pref["camera"]["center"]);
        System::getCamera().mWorldUp = readVector<glm::vec3>(pref["camera"]["world-up"]);

        auto& proj = System::getProjective();
        proj.mFovy = pref["projective"].get("fovy", 45).asFloat();
        proj.mNear = pref["projective"].get("near", 0.1).asFloat();
        proj.mFar  = pref["projective"].get("far", 100).asFloat();

        System::getData().mLightColor = readVector<glm::vec3>(pref["light-color"]);
        System::getData().mLightDir   = readVector<glm::vec3>(pref["light-direction"]);
        System::getData().mClearColor = readVector<glm::vec4>(pref["clear-color"]);

        const auto& models = pref["models"];
        for (Json::Value::ArrayIndex i = 0; i < models.size(); ++i)
        {
            const auto path = models[i].get("path", "").asString();
            const auto modelMeshes = std::make_shared<ModelMeshes>(ModelLoader::load(path));

            std::shared_ptr<btCollisionShape> shape;
            const auto& shapeParameters = models[i]["collision-shape-parameters"];
            const auto shapeType = shapeParameters.get("shape-type", "").asString();
            if (shapeType == "cylinderY")
            {
                float cylinderRadius = shapeParameters.get("radius", 0.0f).asFloat();
                float cylinderHeight = shapeParameters.get("height", 0.0f).asFloat();
                if (cylinderHeight == 0.0f || cylinderRadius == 0.0f)
                {
                    const auto [radius, height] = Utils::calculateCylinderParameters(*modelMeshes, Utils::Axis::Y);
                    cylinderRadius = radius;
                    cylinderHeight = height;
                }
                shape = std::make_shared<btCylinderShape>(btVector3(cylinderRadius, cylinderHeight, cylinderRadius));
            }
            assert(shape);

            const auto& modelsProperties = models[i]["models-properties"];
            for (Json::Value::ArrayIndex j = 0; j < modelsProperties.size(); ++j)
            {
                auto model = Model();
                static size_t id = 0;

                model.mId = id++;
                model.mModelMeshes    = modelMeshes;
                model.mCollisionShape = shape;

                const auto position = readVector<glm::vec3>(modelsProperties[j]["position"]);

                const auto& rotJson = modelsProperties[j]["rotation"];
                const auto rotateAxis = readVector<glm::vec3>(rotJson[0]);
                const float angle = glm::radians(rotJson[1].asFloat());

                const auto scale = readVector<glm::vec3>(modelsProperties[j]["scale"]);

                auto transform = glm::mat4(1.0f);
                transform = glm::translate(transform, position);
                transform = glm::rotate(transform, angle, rotateAxis);
                transform = glm::scale(transform, scale);
                System::getData().mTransformContainer.emplace_back(std::move(transform));

                auto& rigidBodyParameters = modelsProperties[j]["rigid-body-parameters"];
                const auto& rigidBodyType = rigidBodyParameters.get("body-type", "").asString();
                if (rigidBodyType == "dynamic")
                {
                    model.mMotionState = std::make_shared<MotionState>(model.mId);

                    btScalar mass = rigidBodyParameters.get("mass", 1.0f).asFloat();
                    btVector3 inertia;
                    shape->calculateLocalInertia(mass, inertia);
                    model.mRigidBody = std::make_shared<btRigidBody>(mass, model.mMotionState.get(), model.mCollisionShape.get(), inertia);
                }
                else if (rigidBodyType == "static")
                {
                    model.mRigidBody = std::make_shared<btRigidBody>(BT_ZERO, nullptr, model.mCollisionShape.get());
                }
                System::getPhysics().mDynamicWorld.addRigidBody(model.mRigidBody.get());
                System::getData().mModels.emplace_back(std::move(model));
            }
        }
    }

    GLuint loadTexture(const char* aPath) noexcept
    {
        int width, height, numChannels;
        //stbi_set_flip_vertically_on_load(true);
        const auto textureData = stbi_load(aPath, &width, &height, &numChannels, 0);
        if (!textureData)
        {
            std::cerr << "Texture upload was failed" << std::endl;
            exit(-5);
        }

        GLuint textureId;
        glGenTextures(1, &textureId);
        glBindTexture(GL_TEXTURE_2D, textureId);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        GLenum format{};
        if (numChannels == 1)
            format = GL_RED;
        else if (numChannels == 2)
            format = GL_RG;
        else if (numChannels == 3)
            format = GL_RGB;
        else if (numChannels == 4)
            format = GL_RGBA;

        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, textureData);
        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(textureData);

        return textureId;
    }

    size_t getHash(const std::string& aString)
    {
        return std::hash<std::string>()(aString);
    }

    CylinderParameters calculateCylinderParameters(const ModelMeshes& aModelMeshes, Axis aAxis)
    {
        std::vector<glm::vec2> positions2d;

        size_t positionsNum = 0;
        for (const auto& mesh : aModelMeshes)
        {
            positionsNum += mesh.getVertices().size();
        }
        positions2d.reserve(positionsNum);

        for (const auto& mesh : aModelMeshes)
        {
            const auto& vertices = mesh.getVertices();
            for (const auto& vertex : vertices)
            {
                switch (aAxis)
                {
                case Axis::X:
                    positions2d.push_back({ vertex.mPosition.z, vertex.mPosition.x });
                    break;
                case Axis::Y:
                    positions2d.push_back({ vertex.mPosition.x, vertex.mPosition.y });
                    break;
                case Axis::Z:
                    positions2d.push_back({ vertex.mPosition.y, vertex.mPosition.z });
                    break;
                }
            }
        }

        auto min = glm::vec2(std::numeric_limits<float>::max());
        auto max = glm::vec2(std::numeric_limits<float>::min());

        for (const auto& position : positions2d)
        {
            if (position.x < min.x || (position.x == min.x && position.y < min.y))
                min = position;
            if (position.x > max.x || (position.x == max.x && position.y > max.y))
                max = position;
        }

        return CylinderParameters{ (max.x - min.x)/2.0f, (max.y - min.y)/2.0f };
    }
}
