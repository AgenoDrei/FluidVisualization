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
    void prepareData(DataSet* data);
    void compute(GLfloat resolutionX, GLfloat resolutionY, GLfloat resolutionZ);
    DataSet* interpolatedData;
    DataSet* sourceData;
};



