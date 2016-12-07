#pragma once

#include <cstdint>
#include <GL/glew.h>
#include "InterpolationController.h"

class Shader;

class CpuInterpolationController : public InterpolationController {
public:
    CpuInterpolationController(uint32_t interpolationQuality);
    ~CpuInterpolationController();

    void createShader();
    void prepareGpuBuffer(DataSet* data, uint32_t timestepIndex);
    void loadGpuBuffer();
    void renderParticles(Camera* camera, WindowHandler* wHandler);

private:
    GLfloat* buffer;
    GLuint VAO, VBO;
    Shader* shader;
    uint32_t quality;
    uint32_t particleCount;
};



