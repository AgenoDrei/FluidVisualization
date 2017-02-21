#pragma once

#include <cstdint>
#include <vector>
#include "Particle.h"
#include <glm/vec3.hpp>

class Timestep {
private:
    Particle* _particles;
    uint32_t _numberParticles;

    bool isInArea(Particle* p, glm::vec3 center, float area);
public:
    Timestep(uint32_t numberParticles);
    Timestep(Particle* particles, uint32_t numberParticles);
    ~Timestep();

    Particle getParticle(uint32_t index) const;
    Particle* getParticleReference(uint32_t index);
    Particle* getParticleReference(int32_t index);
    uint32_t getSize() const;

    glm::vec4* getPositionDensity();
    glm::vec3 getDimension();

    glm::vec3 getMinimum();
    float getAverageDensityAt(glm::vec3 position, float area);

    Particle* getParticleAt(glm::vec3 position);

    glm::ivec3 getParticleNumberPerDirection();

    bool isIndexValid(int index);
    bool isIndexValid(uint32_t index);
};
