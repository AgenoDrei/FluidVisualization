#pragma once

#include "InterpolationController.h"

class DataSet;
class Shader;

class GpuInterpolationController : InterpolationController{
public:
    GpuInterpolationController(uint32_t q);
    ~GpuInterpolationController();

    DataSet* interpolateData(DataSet* data);

private:
    void prepareData(DataSet* data);
    void compute();
    DataSet* interpolatedData;
    DataSet* sourceData;
    glm::vec3* buffer;
    Shader* shader;
    uint32_t quality, particleCount;
    GLuint VBO, VAO, texture;

};

