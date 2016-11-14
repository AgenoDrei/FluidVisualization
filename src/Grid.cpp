#include <cmath>
#include "Grid.h"
#include "Timestep.h"
#include "GridCell.h"

Grid::Grid(uint32_t size, glm::vec3 dimension) :
    _dimension(dimension) {
    _cells = new GridCell[size];
}

Grid::~Grid() {
    delete [] _cells;
}

Grid* Grid::CreateFromTimestep(Timestep* timestep, float stepSize) {
    auto timestepSize = timestep->getSize();
    auto timestepMinimum = timestep->getMinimum();
    auto gridSize = timestepSize / stepSize;

    auto numberOfElements = ceil(gridSize.x) * ceil(gridSize.y) * ceil(gridSize.z);
    auto result = new Grid(numberOfElements, gridSize);

    for(auto x = 0; x < ceil(gridSize.x); x++) {
        for(auto y = 0; y < ceil(gridSize.y); y++) {
            for(auto z = 0; z < ceil(gridSize.z); z++) {
                auto localPosition = glm::vec3(timestepMinimum.x * x, timestepMinimum.y * y, timestepMinimum.z * z);
                auto density = timestep->getAverageDensityAt(localPosition, stepSize);

                auto resultOffset = x * + y * static_cast<int>(ceil(gridSize.x)) + z * static_cast<int>(ceil(gridSize.y));
                //result->_cells[resultOffset] = GridCell(localPosition, density);
            }
        }
    }

    return result;
}

glm::vec3 Grid::getDimension() {
    return _dimension;
}

GridCell* Grid::getCell(int x, int y, int z) {
    return &_cells[x + y * static_cast<int>(_dimension.x) + z * static_cast<int>(_dimension.y)];
}
