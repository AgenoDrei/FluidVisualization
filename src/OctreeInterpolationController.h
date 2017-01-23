//
// Created by simon on 22.01.17.
//

#pragma once


#include <vector>
#include "InterpolationController.h"
#include "Particle.h"

class DataSet;
class TimeStep;
class OctreeNode;

class OctreeInterpolationController : public InterpolationController{
private:
    void prepareData(DataSet* data);
    void compute();
    void buildOctree(OctreeNode* node);
    DataSet* interpolatedData;
    DataSet* sourceData;
    OctreeNode* root;
public:
    OctreeInterpolationController();
    ~OctreeInterpolationController();
    DataSet* interpolateData(DataSet* data);
};


