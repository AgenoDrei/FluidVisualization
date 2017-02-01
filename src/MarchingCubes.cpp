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

glm::vec3 getNormalAtPoint(GridCell* cell, int index) {
    glm::vec3 result;
    float a, b;
    glm::vec3 c, d;
    switch(index) {
        case 0:
            result.z = (cell->value[0]->density - cell->value[1]->density) / (cell->value[0]->position.z - cell->value[1]->position.z);
            result.x = (cell->value[0]->density - cell->value[4]->density) / (cell->value[0]->position.x - cell->value[4]->position.x);
            result.y = (cell->value[0]->density - cell->value[3]->density) / (cell->value[0]->position.y - cell->value[3]->position.y);
            break;
        case 1:
            result.z = (cell->value[1]->density - cell->value[0]->density) / (cell->value[1]->position.z - cell->value[0]->position.z);
            result.x = (cell->value[1]->density - cell->value[5]->density) / (cell->value[1]->position.x - cell->value[5]->position.x);
            result.y = (cell->value[1]->density - cell->value[2]->density) / (cell->value[1]->position.y - cell->value[2]->position.y);
            break;
        case 2:
            result.z = (cell->value[2]->density - cell->value[3]->density) / (cell->value[2]->position.z - cell->value[3]->position.z);
            result.x = (cell->value[2]->density - cell->value[6]->density) / (cell->value[2]->position.x - cell->value[6]->position.x);
            result.y = (cell->value[2]->density - cell->value[1]->density) / (cell->value[2]->position.y - cell->value[1]->position.y);
            break;
        case 3:
            result.z = (cell->value[3]->density - cell->value[2]->density) / (cell->value[3]->position.z - cell->value[2]->position.z);
            result.x = (cell->value[3]->density - cell->value[7]->density) / (cell->value[3]->position.x - cell->value[7]->position.x);
            result.y = (cell->value[3]->density - cell->value[0]->density) / (cell->value[3]->position.y - cell->value[0]->position.y);
            break;
        case 4:
            result.z = (cell->value[4]->density - cell->value[5]->density) / (cell->value[4]->position.z - cell->value[5]->position.z);
            result.x = (cell->value[4]->density - cell->value[0]->density) / (cell->value[4]->position.x - cell->value[0]->position.x);
            result.y = (cell->value[4]->density - cell->value[7]->density) / (cell->value[4]->position.y - cell->value[7]->position.y);
            break;
        case 5:
            result.z = (cell->value[5]->density - cell->value[4]->density) / (cell->value[5]->position.z - cell->value[4]->position.z);
            result.x = (cell->value[5]->density - cell->value[1]->density) / (cell->value[5]->position.x - cell->value[1]->position.x);
            result.y = (cell->value[5]->density - cell->value[6]->density) / (cell->value[5]->position.y - cell->value[6]->position.y);
            break;
        case 6:
            result.z = (cell->value[6]->density - cell->value[7]->density) / (cell->value[6]->position.z - cell->value[7]->position.z);
            result.x = (cell->value[6]->density - cell->value[2]->density) / (cell->value[6]->position.x - cell->value[2]->position.x);
            result.y = (cell->value[6]->density - cell->value[5]->density) / (cell->value[6]->position.y - cell->value[5]->position.y);
            break;
        case 7:
            result.z = (cell->value[7]->density - cell->value[6]->density) / (cell->value[7]->position.z - cell->value[6]->position.z);
            result.x = (cell->value[7]->density - cell->value[3]->density) / (cell->value[7]->position.x - cell->value[3]->position.x);
            result.y = (cell->value[7]->density - cell->value[4]->density) / (cell->value[7]->position.y - cell->value[4]->position.y);
            break;
        default:
            throw "Argument out of range";
    }

    if(result.length() < 0.0001f) {
        throw "Something wrong?!";
    }

    return result;
}

std::vector<Triangle> MarchingCubes::polygonise(GridCell* cell, float isolevel) {
    auto cubeIndex = getCubeIndex(cell, isolevel);

    if(edgeTable[cubeIndex] == 0) {
        return std::vector<Triangle>();
    }

    glm::vec3 vertexList[12];
    glm::vec3 normalList[12];
    if(edgeTable[cubeIndex] & 1) {
        vertexList[0] = VertexInterp(isolevel, cell->value[0]->position, cell->value[1]->position, cell->value[0]->density, cell->value[1]->density);

        normalList[0] = VertexInterp(isolevel, getNormalAtPoint(cell, 0), getNormalAtPoint(cell, 1), cell->value[0]->density, cell->value[1]->density);
        normalList[0] = glm::normalize(normalList[0]);

        std::cout<<"Normal: x: "<<normalList[0].x<<" y: "<<normalList[0].y<<" z: "<<normalList[0].z<<std::endl;
    }
    if(edgeTable[cubeIndex] & 2) {
        vertexList[1] = VertexInterp(isolevel, cell->value[1]->position, cell->value[2]->position, cell->value[1]->density, cell->value[2]->density);

        normalList[1] = VertexInterp(isolevel, getNormalAtPoint(cell, 1), getNormalAtPoint(cell, 2), cell->value[1]->density, cell->value[2]->density);
        normalList[1] = glm::normalize(normalList[1]);

        std::cout<<"Normal: x: "<<normalList[1].x<<" y: "<<normalList[1].y<<" z: "<<normalList[1].z<<std::endl;
    }
    if(edgeTable[cubeIndex] & 4) {
        vertexList[2] = VertexInterp(isolevel, cell->value[2]->position, cell->value[3]->position, cell->value[2]->density, cell->value[3]->density);

        normalList[2] = VertexInterp(isolevel, getNormalAtPoint(cell, 2), getNormalAtPoint(cell, 3), cell->value[2]->density, cell->value[3]->density);
        normalList[2] = glm::normalize(normalList[2]);

        std::cout<<"Normal: x: "<<normalList[2].x<<" y: "<<normalList[2].y<<" z: "<<normalList[2].z<<std::endl;
    }
    if(edgeTable[cubeIndex] & 8) {
        vertexList[3] = VertexInterp(isolevel, cell->value[3]->position, cell->value[0]->position, cell->value[3]->density, cell->value[0]->density);

        normalList[3] = VertexInterp(isolevel, getNormalAtPoint(cell, 0), getNormalAtPoint(cell, 3), cell->value[0]->density, cell->value[3]->density);
        normalList[3] = glm::normalize(normalList[3]);

        std::cout<<"Normal: x: "<<normalList[3].x<<" y: "<<normalList[3].y<<" z: "<<normalList[3].z<<std::endl;
    }
    if(edgeTable[cubeIndex] & 16) {
        vertexList[4] = VertexInterp(isolevel, cell->value[4]->position, cell->value[5]->position, cell->value[4]->density, cell->value[5]->density);

        normalList[4] = VertexInterp(isolevel, getNormalAtPoint(cell, 4), getNormalAtPoint(cell, 5), cell->value[4]->density, cell->value[5]->density);
        normalList[4] = glm::normalize(normalList[4]);

        std::cout<<"Normal: x: "<<normalList[4].x<<" y: "<<normalList[4].y<<" z: "<<normalList[4].z<<std::endl;
    }
    if(edgeTable[cubeIndex] & 32) {
        vertexList[5] = VertexInterp(isolevel, cell->value[5]->position, cell->value[6]->position, cell->value[5]->density, cell->value[6]->density);

        normalList[5] = VertexInterp(isolevel, getNormalAtPoint(cell, 5), getNormalAtPoint(cell, 6), cell->value[5]->density, cell->value[6]->density);
        normalList[5] = glm::normalize(normalList[5]);

        std::cout<<"Normal: x: "<<normalList[5].x<<" y: "<<normalList[5].y<<" z: "<<normalList[5].z<<std::endl;
    }
    if(edgeTable[cubeIndex] & 64) {
        vertexList[6] = VertexInterp(isolevel, cell->value[6]->position, cell->value[7]->position, cell->value[6]->density, cell->value[7]->density);

        normalList[6] = VertexInterp(isolevel, getNormalAtPoint(cell, 6), getNormalAtPoint(cell, 7), cell->value[6]->density, cell->value[7]->density);
        normalList[6] = glm::normalize(normalList[6]);

        std::cout<<"Normal: x: "<<normalList[6].x<<" y: "<<normalList[6].y<<" z: "<<normalList[6].z<<std::endl;
    }
    if(edgeTable[cubeIndex] & 128) {
        vertexList[7] = VertexInterp(isolevel, cell->value[7]->position, cell->value[4]->position, cell->value[7]->density, cell->value[4]->density);

        normalList[7] = VertexInterp(isolevel, getNormalAtPoint(cell, 4), getNormalAtPoint(cell, 7), cell->value[4]->density, cell->value[7]->density);
        normalList[7] = glm::normalize(normalList[7]);

        std::cout<<"Normal: x: "<<normalList[7].x<<" y: "<<normalList[7].y<<" z: "<<normalList[7].z<<std::endl;
    }
    if(edgeTable[cubeIndex] & 256) {
        vertexList[8] = VertexInterp(isolevel, cell->value[0]->position, cell->value[4]->position, cell->value[0]->density, cell->value[4]->density);

        normalList[8] = VertexInterp(isolevel, getNormalAtPoint(cell, 4), getNormalAtPoint(cell, 0), cell->value[4]->density, cell->value[0]->density);
        normalList[8] = glm::normalize(normalList[8]);

        std::cout<<"Normal: x: "<<normalList[8].x<<" y: "<<normalList[8].y<<" z: "<<normalList[8].z<<std::endl;
    }
    if(edgeTable[cubeIndex] & 512) {
        vertexList[9] = VertexInterp(isolevel, cell->value[1]->position, cell->value[5]->position, cell->value[1]->density, cell->value[5]->density);

        normalList[9] = VertexInterp(isolevel, getNormalAtPoint(cell, 5), getNormalAtPoint(cell, 1), cell->value[5]->density, cell->value[1]->density);
        normalList[9] = glm::normalize(normalList[9]);

        std::cout<<"Normal: x: "<<normalList[9].x<<" y: "<<normalList[9].y<<" z: "<<normalList[9].z<<std::endl;
    }
    if(edgeTable[cubeIndex] & 1024) {
        vertexList[10] = VertexInterp(isolevel, cell->value[2]->position, cell->value[6]->position, cell->value[2]->density, cell->value[6]->density);

        normalList[10] = VertexInterp(isolevel, getNormalAtPoint(cell, 6), getNormalAtPoint(cell, 2), cell->value[6]->density, cell->value[2]->density);
        normalList[10] = glm::normalize(normalList[10]);

        std::cout<<"Normal: x: "<<normalList[10].x<<" y: "<<normalList[10].y<<" z: "<<normalList[10].z<<std::endl;
    }
    if(edgeTable[cubeIndex] & 2048) {
        vertexList[11] = VertexInterp(isolevel, cell->value[3]->position, cell->value[7]->position, cell->value[3]->density, cell->value[7]->density);

        normalList[11] = VertexInterp(isolevel, getNormalAtPoint(cell, 7), getNormalAtPoint(cell, 3), cell->value[7]->density, cell->value[3]->density);
        normalList[11] = glm::normalize(normalList[11]);

        std::cout<<"Normal: x: "<<normalList[11].x<<" y: "<<normalList[11].y<<" z: "<<normalList[11].z<<std::endl;
    }

    std::vector<Triangle> result;
    auto numberTriangles = 0;
    for(auto i = 0; triangleTable[cubeIndex][i] != -1; i += 3) {
        Triangle currentTriangle;
        currentTriangle.Vertices[0].Position = vertexList[triangleTable[cubeIndex][i]];
        currentTriangle.Vertices[0].Normal = normalList[triangleTable[cubeIndex][i]];
        currentTriangle.Vertices[1].Position = vertexList[triangleTable[cubeIndex][i + 1]];
        currentTriangle.Vertices[1].Normal = normalList[triangleTable[cubeIndex][i + 1]];
        currentTriangle.Vertices[2].Position = vertexList[triangleTable[cubeIndex][i + 2]];
        currentTriangle.Vertices[2].Normal = normalList[triangleTable[cubeIndex][i + 2]];
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

    return p;
}

const std::vector<Triangle>& MarchingCubes::getTriangles() const {
    return triangleMesh;
}
