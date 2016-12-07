#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

class Timestep;
class DataSet;

class ParticleGpuLoader {
public:
    ParticleGpuLoader();
    ~ParticleGpuLoader();

    static GLfloat* bufferGPUParticles(DataSet* data, uint32_t timestepIndex, uint32_t particleCount);
    static GLfloat* bufferGPUParticles(glm::vec3* data, uint32_t particleCount);
    static GLuint loadParticlesToGpu(GLfloat* vertices, uint32_t particleCount, uint32_t bufferElementSize, bool loadDensity);
};

