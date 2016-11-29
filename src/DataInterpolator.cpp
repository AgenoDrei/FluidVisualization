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
    const float gridSize = 0.01f;
    const float maxDistance = 0.02f;
    Particle** grid = new Particle*[100 * 100 * 100];
    uint32_t index = 0;
    for(float z = 0.0f; z < 1.0f; z+=gridSize) { //z
        std::cout << "Log> " << z*100 << " Percent done" << std::endl;
        for(float y = 0.0f; y < 1.0f; y+=gridSize) { //y
            for(float x = 0.0f; x < 1.0f; x+=gridSize) { //x
                //std::cout << "\r";
                //std::cout << "Log> At position (" << x << "|" << y << "|" << z << ")!" << std::flush;
                glm::vec3 position = glm::vec3(x, y, z);
                float density;
                float distance = 99.0f;
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
                //std::cout << "Found particle: " << data.getTimestep(0)->getParticle(nearestNeighbor) << " for: " << glm::to_string(position) << std::endl;
                if(distance <= maxDistance && nearestNeighbor != -1) {
                    density = data.getTimestep(0)->getParticle(nearestNeighbor).density;
                    grid[index]= new Particle(position, glm::vec3(0.0f), glm::vec3(0.0f),density, 0);;
                    //data.getTimestep(0)->removeParticle(nearestNeighbor);
                    index++, found++;
                    //std::cout << "Log> Interpolated lattice " << newParticle << " with Particle" << data.getTimestep(0)->getParticle(nearestNeighbor) << "with distance " << distance << std::endl;
                    continue;

                }
                //std::cout << "No particle found with distance: " << distance << std::endl;
                grid[index] = new Particle(position, glm::vec3(0.0f), glm::vec3(0.0f), 0, 0);;
                index++;
            }
        }
    }
    std::cout << "Found lattices: " << found << std::endl;
    Timestep** tmpArray = new Timestep*[1] {new Timestep(*grid, 100*100*100)};
    DataSet* rtn = new DataSet(100*100*100, 1, tmpArray);
    return rtn;


}


