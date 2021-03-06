#include "Particle.h"
#include "glm/ext.hpp"

Particle::Particle() : 
	density(0), 
	pressure(0) {
}

Particle::Particle(glm::vec3 position, glm::vec3 normal, glm::vec3 velocity, float density, float pressure) :
    position(position),
    normal(normal),
    velocity(velocity),
    density(density),
    pressure(pressure) {

}

void Particle::reset() {
	position = glm::vec3(0, 0, 0);
	normal = glm::vec3(0, 0, 0);
	velocity = glm::vec3(0, 0, 0);
	density = 0.0f;
	pressure = 0.0f;
}

std::ostream &operator<<(std::ostream &os, Particle const &particle) {
    //return os<<"Position: "<<glm::to_string(particle.position)<< " Normal: "<<glm::to_string(particle.normal)<<" Velocity: "<<glm::to_string(particle.velocity)<<" Density: "<<particle.density<<" Pressure: "<<particle.pressure;
	return os<<"Position: "<<glm::to_string(particle.position) << " Density: " << particle.density;
}
