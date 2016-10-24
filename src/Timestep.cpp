#include "Timestep.h"
#include "Particle.h"

Timestep::Timestep(uint32_t numberParticles) {
    _particles = new Particle[numberParticles];
}

Timestep::Timestep(Particle* particles) {
    _particles = particles;
}


Timestep::~Timestep() {
    delete[] _particles;
}
