#pragma once;

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

struct Camera
{
    glm::dvec2 oldPos;

    float radius{ 3.0f };
    float pitch{ 45.0f };
    float yaw{ 0.0f };

    glm::vec3 center{ 0.0f };
    glm::vec3 worldUp{ 0.0f, 1.0f, 0.0f };

    float moveSensitivity{ 0.005f };
    float zoomSensitivity{ 0.1f };

    bool leftMousePressed{ false };
    bool rightMousePressed{ false };
};
