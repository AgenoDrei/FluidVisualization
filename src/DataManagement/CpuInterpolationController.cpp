#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext.hpp>
#include "CpuInterpolationController.h"
#include "DataManagement/DataSet.h"
#include "Shader/Shader.h"
#include "Particle.h"
#include "Timestep.h"
#include "Cameras/Camera.h"
#include "WindowHandler.h"

CpuInterpolationController::CpuInterpolationController() :
        interpolatedData( nullptr ) {}

CpuInterpolationController::~CpuInterpolationController() {
    delete [] interpolatedData;
}

void CpuInterpolationController::prepareData(DataSet* data) {
    sourceData = data; //ToDo Load data
}

DataSet* CpuInterpolationController::interpolateData(DataSet *data, GLfloat resolutionX, GLfloat resolutionY, GLfloat resolutionZ) {
    prepareData(data);
    compute(resolutionX, resolutionY, resolutionZ);
    return interpolatedData;
}

void CpuInterpolationController::compute(GLfloat resolutionX, GLfloat resolutionY, GLfloat resolutionZ) {
    long found = 0;

    uint32_t resolution = (uint32_t) (resolutionX > resolutionY ? ( resolutionX > resolutionZ ? resolutionX : resolutionZ ) : ( resolutionY > resolutionZ ? resolutionY : resolutionZ ));

    const float maxDistance = 0.02f;
    auto arraySize = (uint32_t) (resolutionX * resolutionY * resolutionZ);
    Particle* grid = new Particle[arraySize];
    uint32_t index = 0;
    for(auto i = 0u; i < resolutionZ; i++) {
        std::cout << "Log> " << i/(float)resolutionZ * 100 << " Percent done" << std::endl;
        for(auto j = 0u; j < resolutionY; j++) {
            for(auto k = 0u; k < resolutionX; k++) {
                glm::vec3 position = glm::vec3(k/(float)resolution, j/(float)resolution, i/(float)resolution);
                float distance = FLT_MAX;
                uint32_t nearestNeighbor = 0;

                for(uint32_t l = 0; l < sourceData->getTimestep(0)->getSize(); l++) {
                    float tmpDistance = glm::distance(sourceData->getTimestep(0)->getParticle(l).position, position);
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
                    //grid[index]= new Particle(position, glm::vec3(0.0f), glm::vec3(0.0f),density, 0);
                    grid[index].density = sourceData->getTimestep(0)->getParticle(nearestNeighbor).density;
                    grid[index].normal = sourceData->getTimestep(0)->getParticle(nearestNeighbor).normal;
                    //data.getTimestep(0)->removeParticle(nearestNeighbor);
                    found++;
                    //std::cout << "Log> Interpolated lattice " << newParticle << " with Particle" << data.getTimestep(0)->getParticle(nearestNeighbor) << "with distance " << distance << std::endl;
                } else {
                    //std::cout << "No particle found with distance: " << distance << std::endl;
                    grid[index].density = 0;
                    grid[index].normal = glm::vec3(0.0f);
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
