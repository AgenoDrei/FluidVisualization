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
    void prepareData(Timestep* step) override;
    void compute(GLfloat resolutionX, GLfloat resolutionY, GLfloat resolutionZ) override;
    void buildOctree(OctreeNode* node);
    OctreeNode* searchNode(glm::vec3 searchPosition);
    DataSet* interpolatedData;
    DataSet* sourceData;
    Timestep* lastInterpolated;
    OctreeNode* root;
    GLfloat _minSize;
    GLfloat distanceCorrectionFactor;
public:
    OctreeInterpolationController(GLfloat minSize, GLfloat distanceCorrectionFactor);
    ~OctreeInterpolationController();
    DataSet* interpolateData(DataSet* data, GLfloat resolutionX, GLfloat resolutionY, GLfloat resolutionZ) override;
};


