#pragma once

#include <cstdint>
#include <glm/vec3.hpp>

class Particle;

class Timestep {
private:
    Particle* _particles;
    uint32_t _numberParticles;

    bool isInArea(Particle* p, glm::vec3 center, float area);
public:
    Timestep(uint32_t numberParticles);
    Timestep(Particle* particles, uint32_t numberParticles);
    ~Timestep();

    glm::vec3 getSize();
    glm::vec3 getMinimum();
    float getAverageDensityAt(glm::vec3 position, float area);

    Particle* getParticleAt(glm::vec3 position);
};
