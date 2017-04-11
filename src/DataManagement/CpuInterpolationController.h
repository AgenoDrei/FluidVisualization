#pragma once

#include <cstdint>
#include <GL/glew.h>
#include "InterpolationController.h"

class Shader;
class DataSet;

class CpuInterpolationController : public InterpolationController {
public:
    CpuInterpolationController();
    ~CpuInterpolationController();

    DataSet* interpolateData(DataSet* data, GLfloat resolutionX, GLfloat resolutionY, GLfloat resolutionZ);

private:
    void prepareData(Timestep* step);
    void compute(GLfloat resolutionX, GLfloat resolutionY, GLfloat resolutionZ);
    DataSet* interpolatedData;
    Timestep* sourceData;
};



