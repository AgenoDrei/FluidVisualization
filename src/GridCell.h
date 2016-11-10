#pragma once

#include <glm/vec3.hpp>

class GridCell {
public:
    GridCell();
    GridCell(const glm::vec3 &position, float value);

    glm::vec3 position;
    float value;
};

