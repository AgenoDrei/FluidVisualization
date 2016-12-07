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
#include "DataInterpolator.h"

CpuInterpolationController::CpuInterpolationController(uint32_t interpolationQuality) :
        buffer( nullptr ),
        shader( nullptr ),
        quality( interpolationQuality ) {}

CpuInterpolationController::~CpuInterpolationController() {
    delete [] buffer;
}

void CpuInterpolationController::createShader() {
    shader = new Shader("shader/basic.vert", "shader/basic.frag");
}

void CpuInterpolationController::prepareGpuBuffer(DataSet* data, uint32_t timestepIndex) {
    auto interpolatedData = interpolateDataSet(data);

    particleCount = interpolatedData->getNumberParticles();

    buffer = new GLfloat[particleCount * 4];
    Timestep* step = interpolatedData->getTimestep(timestepIndex);
    uint32_t index = 0;
    for(auto j = 0u; j < particleCount; j++) {
        Particle particle = step->getParticle(j);
        buffer[index] = particle.position.x;
        buffer[index + 1] = particle.position.y;
        buffer[index + 2] = particle.position.z;
        buffer[index + 3] = particle.density;
        index += 4;
    }
    //std::cout << "Index: " << index  << std::endl;
}

void CpuInterpolationController::loadGpuBuffer() {
    uint32_t bufferElementSize = 4; // 3 pos floats, 1 density float
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * particleCount * bufferElementSize, buffer, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, bufferElementSize * sizeof(GLfloat), static_cast<GLvoid*>(nullptr));     // Position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, bufferElementSize * sizeof(GLfloat), reinterpret_cast<GLvoid*>(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    std::cout << "VAO Loader: " << VAO << std::endl;
}

void CpuInterpolationController::renderParticles(Camera* camera, WindowHandler* wHandler) {
    shader->use();
    glm::mat4 model;
    model = glm::translate(model, glm::vec3(0.0f));
    glm::mat4 view;
    model = camera->GetViewMatrix();
    glm::mat4 projection;
    projection = glm::perspective(camera->Zoom, wHandler->getWidth()/wHandler->getHeight(), 0.1f, 10.0f);

    // Pass the matrices to the shader
    auto modelLocation = glGetUniformLocation(shader->Program, "model");
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(glGetUniformLocation(shader->Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shader->Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    glBindVertexArray(VAO);
    glDrawArrays(GL_POINTS, 0, particleCount);
    glBindVertexArray(0);
}

DataSet* CpuInterpolationController::interpolateDataSet(DataSet* data) {
    long found = 0;
    uint32_t gridSize = quality;
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

                for(uint32_t i = 0; i < data->getTimestep(0)->getSize(); i++) {
                    float tmpDistance = glm::distance(data->getTimestep(0)->getParticle(i).position, position);
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
                    density = data->getTimestep(0)->getParticle(nearestNeighbor).density;
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
