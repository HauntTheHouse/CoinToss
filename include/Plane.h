#pragma once

#include <glad\glad.h>

class Plane
{
public:
    Plane() = default;
    void init();
    void clear();
    void render() const;

private:
    GLuint VAO{ 0 }, VBO{ 0 };
};
