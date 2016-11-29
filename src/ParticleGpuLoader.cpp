#include <iostream>
#include "ParticleGpuLoader.h"
#include "DataSet.h"
#include "Particle.h"
#include "Timestep.h"

ParticleGpuLoader::ParticleGpuLoader() {}

ParticleGpuLoader::~ParticleGpuLoader() {
    //delete[] _particleDataBuffer;
}

GLfloat* ParticleGpuLoader::bufferGPUParticles(DataSet* data, uint32_t timestepIndex, uint32_t particleCount) {
    GLfloat* vertices = new GLfloat[particleCount*4];
    Timestep* step = data->getTimestep(0);
    uint32_t index = 0;
    for(auto j = 0u; j < particleCount; j++) {
        Particle particle = step->getParticle(j);
        vertices[index] = particle.position.x;
        vertices[index + 1] = particle.position.y;
        vertices[index + 2] = particle.position.z;
        vertices[index + 3] = particle.density;
        index+=4;
    }
    //std::cout << "Index: " << index  << std::endl;
    return vertices;
}

GLuint ParticleGpuLoader::loadParticlesToGpu(GLfloat* vertices, uint32_t particleCount) {
    std::cout << "Vertices size: " << sizeof(vertices) << std::endl;
    GLuint VAO, VBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * particleCount * 4, vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), static_cast<GLvoid*>(nullptr));     // Position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), reinterpret_cast<GLvoid*>(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    std::cout << "VAO Loader: " << VAO << std::endl;
    return VAO;
}
