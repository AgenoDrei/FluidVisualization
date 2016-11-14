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

Particle Timestep::getParticle(uint32_t index) const{
    return _particles[index];
}
