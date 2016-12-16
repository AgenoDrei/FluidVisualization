#pragma once

#include <cstdint>
#include <GL/glew.h>
#include "InterpolationController.h"

class Shader;
class DataSet;

class CpuInterpolationController : public InterpolationController {
public:
    CpuInterpolationController(uint32_t interpolationQuality);
    ~CpuInterpolationController();

    DataSet* interpolateData(DataSet* data);

private:
    void prepareData(DataSet* data);
    void compute();
    DataSet* interpolatedData;
    DataSet* sourceData;
    uint32_t quality;
};



