#include "GridCell.h"

GridCell::GridCell(const glm::vec3 &position, float value) :
        position(position),
        value(value) {
}

GridCell::GridCell() {
    position = glm::vec3(0.0f, 0.0f, 0.0f);
    value = 0.0f;
}
