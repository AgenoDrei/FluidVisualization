#include <glm/gtx/norm.hpp>
#include "Timestep.h"
#include "Particle.h"


Timestep::Timestep(uint32_t numberParticles) {
    _numberParticles = numberParticles;
    _particles = new Particle[numberParticles];
}

Timestep::Timestep(Particle* particles, uint32_t numberParticles) :
    _particles(particles),
    _numberParticles(numberParticles) {

}

Timestep::~Timestep() {
    delete [] _particles;
}

Particle Timestep::getParticle(uint32_t index) const{
    return _particles[index];
}

uint32_t Timestep::getSize() const {
    return _numberParticles;
}

glm::vec4* Timestep::getPositionDensity() {
    auto size = getSize();
    auto result = new glm::vec4[size];

    for(auto i = 0u; i < size; i++) {
        auto particle = getParticle(i);
        result[i].x = particle.position.x;
        result[i].y = particle.position.y;
        result[i].z = particle.position.z;
        result[i].w = particle.density;
    }

    return result;
}

glm::vec3 Timestep::getMinimum() {
    auto minX = FLT_MAX;
    auto minY = FLT_MAX;
    auto minZ = FLT_MAX;

    for(auto i = 0u; i < _numberParticles; i++) {
        if(_particles[i].position.x < minX) {
            minX = _particles[i].position.x;
        }
        if(_particles[i].position.y < minY) {
            minY = _particles[i].position.y;
        }
        if(_particles[i].position.z < minZ) {
            minZ = _particles[i].position.z;
        }
    }

    return glm::vec3(minX, minY, minZ);
}

glm::vec3 Timestep::getDimension() {
    auto maxX = FLT_MIN;
    auto maxY = FLT_MIN;
    auto maxZ = FLT_MIN;

    auto minX = FLT_MAX;
    auto minY = FLT_MAX;
    auto minZ = FLT_MAX;

    for(auto i = 0u; i < _numberParticles; i++) {
        if(_particles[i].position.x < minX) {
            minX = _particles[i].position.x;
        }
        if(_particles[i].position.y < minY) {
            minY = _particles[i].position.y;
        }
        if(_particles[i].position.z < minZ) {
            minZ = _particles[i].position.z;
        }

        if(_particles[i].position.x > maxX) {
            maxX = _particles[i].position.x;
        }
        if(_particles[i].position.y > maxY) {
            maxY = _particles[i].position.y;
        }
        if(_particles[i].position.z > maxZ) {
            maxZ = _particles[i].position.z;
        }
    }

    return glm::vec3(abs(minX) + maxX, abs(minY) + maxY, abs(minZ) + maxZ);
}

float Timestep::getAverageDensityAt(glm::vec3 position, float area) {
    auto result = 0.0f;
    auto numberOfParticlesInArea = 0;

    for(auto i = 0u; i < _numberParticles; i++) {
        if(isInArea(&_particles[i], position, area)) {
            numberOfParticlesInArea++;
            result += _particles[i].density;
        }
    }

    if(numberOfParticlesInArea == 0) {
        return 0.0f;
    }
    return result / numberOfParticlesInArea;
}

bool Timestep::isInArea(Particle* p, glm::vec3 center, float area) {
    return p->position.x > center.x - area && p->position.x < center.x + area &&
           p->position.y > center.y - area && p->position.y < center.y + area &&
           p->position.z > center.z - area && p->position.z < center.x + area;
}

Particle* Timestep::getParticleAt(glm::vec3 position) {
    Particle* closestParticle = &_particles[0]; // start with the first one to avoid nullptr handling
    auto closestDistance = glm::length2(closestParticle->position - position);

    for(auto i = 1; i < _numberParticles; i++) { // start with the second one because the first is preselected
        auto currentParticle = &_particles[i];
        auto currentDistance = glm::length2(currentParticle->position - position);

        if(currentDistance < closestDistance) {
            closestDistance = currentDistance;
            closestParticle = currentParticle;
        }
    }

    return closestParticle;
}
