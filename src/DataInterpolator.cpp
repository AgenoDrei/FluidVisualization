//
// Created by simon on 16.11.16.
//

#include <iostream>
#include <glm/gtc/vec1.hpp>
#include <glm/ext.hpp>
#include "DataInterpolator.h"
#include "Timestep.h"

//Remove Magicnumbers
DataSet* DataInterpolator::interpolateDataset(DataSet data) {
    long found = 0;
    uint32_t gridSize = 50;
    const float maxDistance = 0.02f;
    auto arraySize = gridSize * gridSize * gridSize;
    Particle* grid = new Particle[arraySize];
    uint32_t index = 0;
    for(float z = 0.0f; z < 1.0f; z+= 1.0f / gridSize) { //z
        std::cout << "Log> " << z*100 << " Percent done" << std::endl;
        for(float y = 0.0f; y < 1.0f; y+= 1.0f / gridSize) { //y
            for(float x = 0.0f; x < 1.0f; x+= 1.0f / gridSize) { //x
                //std::cout << "\r";
                //std::cout << "Log> At position (" << x << "|" << y << "|" << z << ")!" << std::flush;
                glm::vec3 position = glm::vec3(x, y, z);
                float density;
                float distance = FLT_MAX;
                uint32_t nearestNeighbor = -1;

                for(uint32_t i = 0; i < data.getTimestep(0)->getSize(); i++) {
                    float tmpDistance = glm::distance(data.getTimestep(0)->getParticle(i).position, position);
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
                    density = data.getTimestep(0)->getParticle(nearestNeighbor).density;
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
    /*Timestep** tmpArray = new Timestep*[1] {new Timestep(*grid, arraySize)};
    DataSet* rtn = new DataSet(arraySize, 1, tmpArray);
    return rtn;*/

    auto timesteps = new Timestep*[1];
    timesteps[0] = new Timestep(grid, arraySize);
    auto result = new DataSet(arraySize, 1, timesteps);

    return result;
}


