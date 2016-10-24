#pragma once

#include <cstdint>

class Particle;

class Timestep {
private:
    Particle* _particles;

public:
    Timestep(uint32_t numberParticles);
    Timestep(Particle* particles);
    ~Timestep();
};
