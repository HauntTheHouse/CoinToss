#pragma once
#include <glm/vec4.hpp>

struct Data
{
    Data() = default;

    void init(const glm::vec4& aClearColor);

    glm::vec4 mClearColor;
};
