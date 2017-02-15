//
// Created by simon on 07.12.16.
//

#include <glm/vec3.hpp>
#include <GL/glew.h>
#include <iostream>
#include <glm/detail/type_mat.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext.hpp>
#include "GpuInterpolationController.h"
#include "Shader.h"
#include "Camera.h"
#include "WindowHandler.h"

GpuInterpolationController::GpuInterpolationController(uint32_t q) :
        buffer( nullptr ),
        shader( nullptr ),
        quality( q ){
    shader = new Shader("shader/interpolate.vert", "shader/interpolate.frag");
}

GpuInterpolationController::~GpuInterpolationController() {
    delete [] buffer;
}

void GpuInterpolationController::prepareData(DataSet *data) {
    sourceData = data;

    auto gridSize = quality;
    particleCount = gridSize * gridSize * gridSize;
    buffer = new glm::vec3[particleCount];
    auto index = 0;

    for(auto i = 0u; i < gridSize; i++) {
        for (auto j = 0u; j < gridSize; j++) {
            for (auto k = 0u; k < gridSize; k++) {
                buffer[index] = glm::vec3(i / (float) gridSize, j / (float) gridSize, k / (float) gridSize);
                index++;
            }
        }
    }

    uint32_t bufferElementSize = 3; // 3 pos floats
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * particleCount * bufferElementSize, buffer, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, bufferElementSize * sizeof(GLfloat), static_cast<GLvoid*>(nullptr));     // Position attribute
    glEnableVertexAttribArray(0);
    //glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, bufferElementSize * sizeof(GLfloat), reinterpret_cast<GLvoid*>(3 * sizeof(GLfloat)));
    //glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    //Texture Stuff
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	// Set texture wrapping to GL_REPEAT (usually basic wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA4, glm::sqrt(particleCount), glm::sqrt(particleCount), 0, GL_RGBA, GL_FLOAT, buffer);

    glBindTexture(GL_TEXTURE_2D, 0);

}


void GpuInterpolationController::compute() {
    glBindTexture(GL_TEXTURE_2D, texture);
    shader->use();
    /*glm::mat4 model;
    model = glm::translate(model, glm::vec3(0.0f));
    glm::mat4 view;
    model = camera->GetViewMatrix();
    glm::mat4 projection;
    projection = glm::perspective(camera->Zoom, wHandler->getWidth()/wHandler->getHeight(), 0.1f, 10.0f);

    // Pass the matrices to the shader
    auto modelLocation = glGetUniformLocation(shader->Program, "model");
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(glGetUniformLocation(shader->Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shader->Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));*/

    glUniform1i(glGetUniformLocation(shader->Program, "size"), 250000);
    glUniform1f(glGetUniformLocation(shader->Program, "maxDistance"), 0.02f);

    glBindVertexArray(VAO);
    glDrawArrays(GL_POINTS, 0, particleCount);
    glBindVertexArray(0);
}

DataSet* GpuInterpolationController::interpolateData(DataSet *data) {
    prepareData(data);
    compute();

    return interpolatedData;
}