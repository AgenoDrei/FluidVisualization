//
// Created by simon on 07.12.16.
//

#include <glm/ext.hpp>
#include "RendererParticles.h"
#include "DataSet.h"
#include "Timestep.h"
#include "glm/vec4.hpp"
#include "WindowHandler.h"
#include "Camera.h"
#include "Shader.h"

RendererParticles::RendererParticles() {
    shader = new Shader("shader/basic.vert", "shader/basic.frag");
}

RendererParticles::~RendererParticles() {
    delete [] buffer;
}

void RendererParticles::setData(Timestep* step, uint32_t count) {
    particleCount = count;

    buffer = new glm::vec4[particleCount];
    uint32_t index = 0;
    for(auto j = 0u; j < particleCount; j++) {
        Particle particle = step->getParticle(j);
        buffer[index] = glm::vec4(particle.position, particle.density);
        index++;
    }

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

}

void RendererParticles::render(Camera *camera, WindowHandler *wHandler) {
    shader->use();
    glm::mat4 model;
    model = glm::translate(model, glm::vec3(0.0f));
    glm::mat4 view;
    view = camera->GetViewMatrix();
    glm::mat4 projection;
    projection = camera->GetProjectonMatrix(wHandler, 0.1f, 10.0f);

    // Pass the matrices to the shader
    auto modelLocation = glGetUniformLocation(shader->Program, "model");
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(glGetUniformLocation(shader->Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shader->Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    glBindVertexArray(VAO);
    glDrawArrays(GL_POINTS, 0, particleCount);
    glBindVertexArray(0);
}