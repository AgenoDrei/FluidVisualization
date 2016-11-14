#pragma once

#include <cstdint>
#include <glm/vec3.hpp>

class GridCell;
class Timestep;

class Grid {
private:
    GridCell* _cells;
    glm::vec3 _dimension;
public:
    Grid(uint32_t size, glm::vec3 dimension);
    ~Grid();

    static Grid* CreateFromTimestep(Timestep* timestep, float stepSize);

    glm::vec3 getDimension();

    GridCell* getCell(int x, int y, int z);
};



