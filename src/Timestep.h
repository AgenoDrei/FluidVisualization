#pragma once

#include <cstdint>
#include "Particle.h"

class Timestep {
private:
    Particle* _particles;

public:
    Timestep(uint32_t numberParticles);
    Timestep(Particle* particles);
    ~Timestep();

    Particle getParticle(uint32_t index) const;
};
