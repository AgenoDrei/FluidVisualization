#pragma once

#include <glm/glm.hpp>
#include <ostream>

class Particle {
public:
    Particle();
    Particle(glm::vec3 position, glm::vec3 normal, glm::vec3 velocity, float density, float pressure);

    glm::vec3 position;
    glm::vec3 normal;
    glm::vec3 velocity;
    float density;
    float pressure;
};

std::ostream &operator<<(std::ostream &os, Particle const &particle);
