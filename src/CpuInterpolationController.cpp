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
    auto interpolatedData = DataInterpolator::interpolateDataset(*data, quality);

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
