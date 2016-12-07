#pragma once

#include <cstdint>
#include <vector>
#include "Particle.h"



class Timestep {
private:
    std::vector<Particle> _particles;

public:
    Timestep(uint32_t numberParticles);
    Timestep(Particle* particles, uint32_t numberParticles);
    ~Timestep();

    Particle getParticle(uint32_t index) const;
    uint32_t getSize() const;
    void removeParticle(uint32_t index);

    glm::vec4* getPositionDensity();
};
