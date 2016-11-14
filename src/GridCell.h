#pragma once

#include <glm/vec3.hpp>

class Particle;

class GridCell {
public:
    GridCell();
    GridCell(const glm::vec3 &position, Particle* value[8]);

    glm::vec3 position;
    Particle* value[8];
};

