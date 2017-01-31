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
    return x >= 0 && x + 1 < _dimension.x && y >= 0 && y + 1 < _dimension.y && z >= 0 && z + 1 < _dimension.z;
}

bool Grid::isValidIndex(int x, int y, int z) {
    return x >= 0 && x < _dimension.x && y >= 0 && y < _dimension.y && z >= 0 && z < _dimension.z;
}

#include <iostream>
GridCell* Grid::getCell(int x, int y, int z) {
    auto particles = new Particle*[8];

    int indices[8][3] = {
            {0, 0, 0},
            {1, 0, 0},
            {1, 1, 0},
            {0, 1, 0},
            {0, 0, 1},
            {1, 0, 1},
            {1, 1, 1},
            {0, 1, 1},
    };

    for(auto i = 0u; i <= 7u; i++) {
        particles[i] = _timestep->getParticleReference(getIndex(x + indices[i][0], y + indices[i][1], z + indices[i][2]));
    }

    return new GridCell(glm::ivec3(x, y, z), particles);
}
