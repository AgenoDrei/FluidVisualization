//
// Created by simon on 20.01.17.
//

#include "DataExporter.h"
#include "DataSet.h"
#include "Timestep.h"
#include <iostream>

bool DataExporter::write(std::string path, DataSet* data) {
    std::ofstream file(path.c_str(), std::ios::binary | std::ios::trunc);
    if(!file.is_open()) {
        file.close();
        throw "Could not create File";
    }

    uint32_t numberParticles = data->getNumberParticles();
    uint32_t numberTimesteps = data->getNumberTimesteps();

    file.write(reinterpret_cast<char*>(&numberTimesteps), 4);
    file.write(reinterpret_cast<char*>(&numberParticles), 4);


    for(auto i = 0u; i < numberTimesteps; i++) {
        writeTimestep(file, numberParticles, data->getTimestep(i));
    }

    file.close();
    return true;
}

void DataExporter::writeTimestep(std::ofstream &file, int32_t numberParticles, Timestep *timestep) {
    for(auto i = 0; i < numberParticles; i++) {
        writeParticle(file, timestep->getParticle(i));
    }
}

void DataExporter::writeParticle(std::ofstream &file, Particle particle) {
    glm::vec3 position = particle.position;
    glm::vec3 normal = particle.normal;
    glm::vec3 velocity = particle.velocity;
    float density = particle.density;
    float pressure = particle.pressure;

    file.write(reinterpret_cast<char*>(&position), sizeof(glm::vec3));
    file.write(reinterpret_cast<char*>(&normal), sizeof(glm::vec3));
    file.write(reinterpret_cast<char*>(&velocity), sizeof(glm::vec3));
    file.write(reinterpret_cast<char*>(&density), sizeof(float));
    file.write(reinterpret_cast<char*>(&pressure), sizeof(float));
}