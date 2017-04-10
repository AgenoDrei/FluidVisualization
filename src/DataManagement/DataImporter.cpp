#include "DataImporter.h"
#include "Timestep.h"
#include "Particle.h"
#include "DataSet.h"
#include <iostream>
#include <glm/ext.hpp>

DataSet* DataImporter::load(std::string path) {
    std::ifstream file(path.c_str(), std::ifstream::binary);
    if(file.fail()) {
        file.close();
        throw "File not found";
    }

    uint32_t numberParticles = 0;
    uint32_t numberTimesteps = 0;

    file.read(reinterpret_cast<char*>(&numberTimesteps), 4);
    file.read(reinterpret_cast<char*>(&numberParticles), 4);

    std::cout<<"Found: "<<numberTimesteps<<" timesteps"<<std::endl;
    std::cout<<"Found: "<<numberParticles<<" particles per timestep"<<std::endl;

    auto timesteps = new Timestep*[numberTimesteps];
    for(auto i = 0u; i < numberTimesteps; i++) {
        timesteps[i] = loadTimestep(file, numberParticles);
    }

    file.close();

    std::cout<<"Dimensions: "<<glm::to_string(timesteps[0]->getParticleNumberPerDirection())<<std::endl;

	return new DataSet(numberParticles, numberTimesteps, timesteps);
}

Timestep* DataImporter::loadTimestep(std::ifstream& file, uint32_t numberParticles) {
    auto particles = new Particle[numberParticles];
    for(auto i = 0u; i < numberParticles; i++) {
        particles[i] = loadParticle(file);
    }

    return new Timestep(particles, numberParticles);
}

Particle DataImporter::loadParticle(std::ifstream& file) {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec3 velocity;
    float density;
    float pressure;

    file.read(reinterpret_cast<char*>(&position), sizeof(glm::vec3));
    file.read(reinterpret_cast<char*>(&normal), sizeof(glm::vec3));
    file.read(reinterpret_cast<char*>(&velocity), sizeof(glm::vec3));
    file.read(reinterpret_cast<char*>(&density), sizeof(float));
    file.read(reinterpret_cast<char*>(&pressure), sizeof(float));

    auto particle = Particle(position, normal, velocity, density, pressure);
    return particle;
}
