#pragma once

#include <glm/vec4.hpp>

class BaseLight {
protected:
    BaseLight();
public:
    glm::vec4 color;

    virtual ~BaseLight();
};



