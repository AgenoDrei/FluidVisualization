//
// Created by simon on 07.12.16.
//

#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext.hpp>
#include "CpuInterpolationController.h"
#include "DataSet.h"
#include "Shader.h"
#include "Particle.h"
#include "Timestep.h"
#include "Camera.h"
#include "WindowHandler.h"

CpuInterpolationController::CpuInterpolationController(uint32_t interpolationQuality) :
        interpolatedData( nullptr ),
        quality( interpolationQuality ) {}

CpuInterpolationController::~CpuInterpolationController() {
    delete [] interpolatedData;
}

void CpuInterpolationController::prepareData(DataSet* data) {
    sourceData = data; //ToDo Load data
}

DataSet* CpuInterpolationController::interpolateData(DataSet *data) {
    prepareData(data);
    compute();
    return interpolatedData;
}

void CpuInterpolationController::compute() {
    long found = 0;
    uint32_t gridSize = quality;
    const float maxDistance = 0.02f;
    auto arraySize = gridSize * gridSize * gridSize;
    Particle* grid = new Particle[arraySize];
    uint32_t index = 0;
    for(auto i = 0u; i < gridSize; i++) {
        std::cout << "Log> " << i/(float)gridSize * 100 << " Percent done" << std::endl;
        for(auto j = 0u; j < gridSize; j++) {
            for(auto k = 0u; k < gridSize; k++) {
                float z =  i/(float)gridSize;
                float y =  j/(float)gridSize;
                float x =  k/(float)gridSize;

                glm::vec3 position = glm::vec3(x, y, z);
                float density;
                float distance = FLT_MAX;
                uint32_t nearestNeighbor = -1;

                for(uint32_t i = 0; i < sourceData->getTimestep(0)->getSize(); i++) {
                    float tmpDistance = glm::distance(sourceData->getTimestep(0)->getParticle(i).position, position);
                    if(tmpDistance < distance) {
                        distance = tmpDistance;
                        nearestNeighbor = i;
                    }
                    /*if(distance < 0.01f) {
                        break;
                    }*/
                }
                grid[index].reset();
                grid[index].position = position;
                //std::cout << "Found particle: " << data.getTimestep(0)->getParticle(nearestNeighbor) << " for: " << glm::to_string(position) << std::endl;
                if(distance <= maxDistance && nearestNeighbor != -1u) {
                    density = sourceData->getTimestep(0)->getParticle(nearestNeighbor).density;
                    //grid[index]= new Particle(position, glm::vec3(0.0f), glm::vec3(0.0f),density, 0);
                    grid[index].density = density;
                    //data.getTimestep(0)->removeParticle(nearestNeighbor);
                    found++;
                    //std::cout << "Log> Interpolated lattice " << newParticle << " with Particle" << data.getTimestep(0)->getParticle(nearestNeighbor) << "with distance " << distance << std::endl;
                } else {
                    //std::cout << "No particle found with distance: " << distance << std::endl;
                    grid[index].density = 0;
                    //grid[index] = new Particle(position, glm::vec3(0.0f), glm::vec3(0.0f), 0, 0);
                }
                //std::cout<<"Position: "<<grid[index]->position.x<< " "<<grid[index]->position.y<<" "<<grid[index]->position.z<<std::endl;
                index++;
            }
        }
    }

    std::cout << "Found lattices: " << found << std::endl;
    auto timesteps = new Timestep*[1];
    timesteps[0] = new Timestep(grid, arraySize);
    auto result = new DataSet(arraySize, 1, timesteps);

    interpolatedData = result;

}
