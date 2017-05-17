#pragma once

#include <glm/vec3.hpp>
#include "BaseLight.h"

class DirectionalLight : public BaseLight {
public:
    DirectionalLight();
    DirectionalLight(glm::vec3 direction);

    glm::vec3 direction;
};



