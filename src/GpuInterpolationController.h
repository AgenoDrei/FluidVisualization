#pragma once

#include "InterpolationController.h"

class DataSet;
class Shader;

class GpuInterpolationController : InterpolationController{
public:
    GpuInterpolationController(uint32_t interpolationQuality);
    ~GpuInterpolationController();

    DataSet* interpolateData(DataSet* data);

private:
    void prepareData(DataSet* data);
    void compute();
    DataSet* interpolatedData;
    DataSet* sourceData;
    uint32_t quality;
};

