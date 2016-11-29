#include "Timestep.h"
#include "Particle.h"


Timestep::Timestep(uint32_t numberParticles) {
}

Timestep::Timestep(Particle* particles, uint32_t numberParticles) {
    _particles.assign(particles, particles + numberParticles);
}


Timestep::~Timestep() {
    _particles.clear();
}

Particle Timestep::getParticle(uint32_t index) const{
    return _particles.at(index);
}

uint32_t Timestep::getSize() const {
    return _particles.size();
}

void Timestep::removeParticle(uint32_t index) {
    _particles.erase(_particles.begin() + index);
}