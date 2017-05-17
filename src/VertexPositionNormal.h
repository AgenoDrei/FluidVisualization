#pragma once

#include <glm/vec3.hpp>

class VertexPositionNormal {
public:
    glm::vec3 Position;
    glm::vec3 Normal;

    bool operator==(const VertexPositionNormal &other) const;
    bool operator!=(const VertexPositionNormal &other) const;
};



