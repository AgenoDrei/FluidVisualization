#pragma once

#include <cstdint>

class GridCell;
class Timestep;

class Grid {
private:
    GridCell* _cells;
public:
    Grid(uint32_t size);
    ~Grid();

    static Grid* CreateFromTimestep(Timestep* timestep, float stepSize);
};



