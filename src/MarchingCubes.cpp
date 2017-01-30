#include "MarchingCubes.h"
#include "Timestep.h"
#include "Grid.h"
#include "GridCell.h"
#include "MarchinCubesData.h"
#include <iostream>

MarchingCubes::MarchingCubes() {

}

void MarchingCubes::calculate(Grid* grid) {
    triangleMesh.clear();

    auto gridDimension = grid->getDimension();
    for(auto x = 0; x < gridDimension.x; x++) {
        for(auto y = 0; y < gridDimension.y; y++) {
            for(auto z = 0; z < gridDimension.z; z++) {
                if(grid->isValidCell(x, y , z)) {
                    auto meshPart = polygonise(grid->getCell(x, y, z), 0.000001f);
                    triangleMesh.insert(triangleMesh.end(), meshPart.begin(), meshPart.end());
                }
            }
        }
    }
    std::cout<<"Marching cubes found "<<triangleMesh.size()<<" triangles"<<std::endl;
}

int MarchingCubes::getCubeIndex(GridCell* cell, float isolevel) {
    auto cubeIndex = 0;

    if(cell->value[0]->density < isolevel) {
        cubeIndex |= 1;
    }
    if(cell->value[1]->density < isolevel) {
        cubeIndex |= 2;
    }
    if(cell->value[2]->density < isolevel) {
        cubeIndex |= 4;
    }
    if(cell->value[3]->density < isolevel) {
        cubeIndex |= 8;
    }
    if(cell->value[4]->density < isolevel) {
        cubeIndex |= 16;
    }
    if(cell->value[5]->density < isolevel) {
        cubeIndex |= 32;
    }
    if(cell->value[6]->density < isolevel) {
        cubeIndex |= 64;
    }
    if(cell->value[7]->density < isolevel) {
        cubeIndex |= 128;
    }

    return cubeIndex;
}

std::vector<Triangle> MarchingCubes::polygonise(GridCell* cell, float isolevel) {
    auto cubeIndex = getCubeIndex(cell, isolevel);

    if(edgeTable[cubeIndex] == 0) {
        return std::vector<Triangle>();
    }

    glm::vec3 vertexList[12];
    if(edgeTable[cubeIndex] & 1) {
        vertexList[0] = VertexInterp(isolevel, cell->value[0]->position, cell->value[1]->position, cell->value[0]->density, cell->value[1]->density);
    }
    if(edgeTable[cubeIndex] & 2) {
        vertexList[1] = VertexInterp(isolevel, cell->value[1]->position, cell->value[2]->position, cell->value[1]->density, cell->value[2]->density);
    }
    if(edgeTable[cubeIndex] & 4) {
        vertexList[2] = VertexInterp(isolevel, cell->value[2]->position, cell->value[3]->position, cell->value[2]->density, cell->value[3]->density);
    }
    if(edgeTable[cubeIndex] & 8) {
        vertexList[3] = VertexInterp(isolevel, cell->value[3]->position, cell->value[0]->position, cell->value[3]->density, cell->value[0]->density);
    }
    if(edgeTable[cubeIndex] & 16) {
        vertexList[4] = VertexInterp(isolevel, cell->value[4]->position, cell->value[5]->position, cell->value[4]->density, cell->value[5]->density);
    }
    if(edgeTable[cubeIndex] & 32) {
        vertexList[5] = VertexInterp(isolevel, cell->value[5]->position, cell->value[6]->position, cell->value[5]->density, cell->value[6]->density);
    }
    if(edgeTable[cubeIndex] & 64) {
        vertexList[6] = VertexInterp(isolevel, cell->value[6]->position, cell->value[7]->position, cell->value[6]->density, cell->value[7]->density);
    }
    if(edgeTable[cubeIndex] & 128) {
        vertexList[7] = VertexInterp(isolevel, cell->value[7]->position, cell->value[4]->position, cell->value[7]->density, cell->value[4]->density);
    }
    if(edgeTable[cubeIndex] & 256) {
        vertexList[8] = VertexInterp(isolevel, cell->value[0]->position, cell->value[4]->position, cell->value[0]->density, cell->value[4]->density);
    }
    if(edgeTable[cubeIndex] & 512) {
        vertexList[9] = VertexInterp(isolevel, cell->value[1]->position, cell->value[5]->position, cell->value[1]->density, cell->value[5]->density);
    }
    if(edgeTable[cubeIndex] & 1024) {
        vertexList[10] = VertexInterp(isolevel, cell->value[2]->position, cell->value[6]->position, cell->value[2]->density, cell->value[6]->density);
    }
    if(edgeTable[cubeIndex] & 2048) {
        vertexList[11] = VertexInterp(isolevel, cell->value[3]->position, cell->value[7]->position, cell->value[3]->density, cell->value[7]->density);
    }

    std::vector<Triangle> result;
    auto numberTriangles = 0;
    for(auto i = 0; triangleTable[cubeIndex][i] != -1; i += 3) {
        Triangle currentTriangle;
        currentTriangle.Position[0] = vertexList[triangleTable[cubeIndex][i]];
        currentTriangle.Position[1] = vertexList[triangleTable[cubeIndex][i + 1]];
        currentTriangle.Position[2] = vertexList[triangleTable[cubeIndex][i + 2]];
        numberTriangles++;

        result.push_back(currentTriangle);
    }

    return result;
}

glm::vec3 MarchingCubes::VertexInterp(float isolevel, glm::vec3 p1, glm::vec3 p2, float vlaueP1, float vlaueP2) {
    float mu;
    glm::vec3 p;

    if(std::abs(isolevel - vlaueP1) < INTERPOLATION_DELTA) {
        return p1;
    }
    if(std::abs(isolevel - vlaueP2) < INTERPOLATION_DELTA) {
        return p2;
    }
    if(std::abs(vlaueP1 - vlaueP2) < INTERPOLATION_DELTA) {
        return p1;
    }
    mu = (isolevel - vlaueP1) / (vlaueP2 - vlaueP1);
    p.x = p1.x + mu * (p2.x - p1.x);
    p.y = p1.y + mu * (p2.y - p1.y);
    p.z = p1.z + mu * (p2.z - p1.z);

    return(p);
}

const std::vector<Triangle>& MarchingCubes::getTriangles() const {
    return triangleMesh;
}
