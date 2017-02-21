#include "GridCell.h"

GridCell::GridCell(const glm::ivec3 &position, Particle* value[8]) :
        position(position) {
    for(auto i = 0; i < 8; i++) {
        this->value[i] = value[i];
    }
}

GridCell::GridCell() {
    position = glm::vec3(0.0f, 0.0f, 0.0f);
}
