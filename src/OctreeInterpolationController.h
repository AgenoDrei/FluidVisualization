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
    OctreeNode* searchNode(glm::vec3 searchPosition);
    DataSet* interpolatedData;
    DataSet* sourceData;
    OctreeNode* root;
    uint32_t resolution, searchDepth;
public:
    OctreeInterpolationController(uint32_t quality, uint32_t depth);
    ~OctreeInterpolationController();
    DataSet* interpolateData(DataSet* data);
};


