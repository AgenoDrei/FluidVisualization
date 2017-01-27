#include <cmath>
#include "Grid.h"
#include "Timestep.h"
#include "GridCell.h"

Grid::Grid(glm::ivec3 dimension, Timestep* timestep) :
    _dimension(dimension),
    _timestep(timestep) {
}

Grid::~Grid() {
}

Grid* Grid::CreateFromTimestep(Timestep* timestep, float stepSize) {
    // Not implemented
    return nullptr;
}

glm::vec3 Grid::getDimension() {
    return _dimension;
}

int Grid::getIndex(int x, int y, int z) {
    //return x + y * _dimension.x + z * _dimension.x * _dimension.y;
    return x * _dimension.y * _dimension.z + y * _dimension.z + z;
}

bool Grid::isValidCell(int x, int y, int z) {
    return x >= 0 && x + 1 < _dimension.x && y - 1 >= 0 && y < _dimension.y && z >= 0 && z + 1 < _dimension.z;
}

#include <iostream>
GridCell* Grid::getCell(int x, int y, int z) {
    if(!isValidCell(x, y, z)) {
        throw "Invalid argument";
    }

    auto particles = new Particle*[8];
    particles[0] = _timestep->getParticleReference(getIndex(x, y, z));
    particles[1] = _timestep->getParticleReference(getIndex(x + 1, y, z));
    particles[2] = _timestep->getParticleReference(getIndex(x + 1, y - 1, z));
    particles[3] = _timestep->getParticleReference(getIndex(x, y - 1, z));
    particles[4] = _timestep->getParticleReference(getIndex(x, y, z + 1));
    particles[5] = _timestep->getParticleReference(getIndex(x + 1, y, z + 1));
    particles[6] = _timestep->getParticleReference(getIndex(x + 1, y - 1, z + 1));
    particles[7] = _timestep->getParticleReference(getIndex(x, y - 1, z + 1));

    return new GridCell(glm::ivec3(x, y, z), particles);
}
