#pragma once

#include <cstdint>
#include <glm/vec3.hpp>

class GridCell;
class Timestep;

class Grid {
private:
    glm::ivec3 _dimension;
    Timestep* _timestep;
protected:
    int getIndex(int x, int y, int z);
public:
    Grid(glm::ivec3 dimension, Timestep* timestep); // the timestep is already in a grid format
    ~Grid();

    static Grid* CreateFromTimestep(Timestep* timestep, float stepSize);

    glm::vec3 getDimension();

    GridCell* getCell(int x, int y, int z);

    bool isValidCell(int x, int y, int z);
    bool isValidIndex(int x, int y, int z);
};



