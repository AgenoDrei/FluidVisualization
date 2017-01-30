#pragma once

#include "Triangle.h"
#include <vector>
#include <glm/vec3.hpp>

class Timestep;
class GridCell;
class Grid;
class Timestep;

#define INTERPOLATION_DELTA 0.00001f

class MarchingCubes {
protected:
    std::vector<Triangle> triangleMesh;

    std::vector<Triangle> polygonise(GridCell* cell, float isolevel);
    int getCubeIndex(GridCell* cell, float isolevel);

    glm::vec3 VertexInterp(float isolevel, glm::vec3 p1, glm::vec3 p2, float vlaueP1, float vlaueP2);
public:
    MarchingCubes();

    void calculate(Grid* grid);
    const std::vector<Triangle>& getTriangles() const;
};



