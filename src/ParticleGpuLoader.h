#pragma once

#include <GL/glew.h>

class Timestep;
class DataSet;

class ParticleGpuLoader {
public:
    ParticleGpuLoader();
    ~ParticleGpuLoader();

    static GLfloat* bufferGPUParticles(DataSet* data, uint32_t timestepIndex, uint32_t particleCount);
    static GLuint loadParticlesToGpu(GLfloat* vertices, uint32_t particleCount);
};

