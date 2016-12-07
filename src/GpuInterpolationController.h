#pragma once

#include "InterpolationController.h"

class DataSet;
class Shader;

class GpuInterpolationController : InterpolationController{
public:
    GpuInterpolationController(uint32_t q);
    ~GpuInterpolationController();

    void createShader();
    void prepareGpuBuffer(DataSet* data, uint32_t timestepIndex);
    void loadGpuBuffer();
    void renderParticles(Camera* camera, WindowHandler* wHandler);

private:
    glm::vec3* buffer;
    GLuint VAO, VBO;
    Shader* shader;
    uint32_t particleCount;
    uint32_t quality;
};

