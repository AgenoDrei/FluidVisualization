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

glm::vec4* Timestep::getPositionDensity() {
    auto result = new glm::vec4[getSize()];

    for(auto i = 0u; i < getSize(); i++) {
        auto particle = getParticle(i);
        result[i].x = particle.position.x;
        result[i].y = particle.position.y;
        result[i].z = particle.position.z;
        result[i].w = particle.density;
    }

    return result;
}